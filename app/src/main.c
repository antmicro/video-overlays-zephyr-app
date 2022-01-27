/*
 * Copyright (c) 2021-2022 Antmicro
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <device.h>
#include <net/net_ip.h>
#include <stdio.h>
#include <net/socket.h>
#include <drivers/video.h>
#include <drivers/dma.h>
#include <drivers/gpio.h>
#include <timing/timing.h>

#include <logging/log.h>
LOG_MODULE_REGISTER(app);

#include "net.h"

#define OV2640_1 "OV2640_1"
#define OV2640_2 "OV2640_2"
#define FASTVDMA_1 "FASTVDMA_1"
#define FASTVDMA_2 "FASTVDMA_2"
#define FASTVDMA_GPU_IN_1 "FASTVDMA_GPU_IN_1"
#define FASTVDMA_GPU_IN_2 "FASTVDMA_GPU_IN_2"
#define FASTVDMA_GPU_OUT "FASTVDMA_GPU_OUT"
#define GPIO_EXPANDER "PCA9500_GPIO"

#define MY_STACK_SIZE 500
#define MY_PRIORITY 5

/* Implement function to flush cache memory for DMA */
#define CONFIG_L2_SIZE 8192
#define MAIN_RAM_BASE 0x40000000L

#define STACKSIZE 1024
#define PRIORITY 7

const struct device *ov2640_dev_1;
const struct device *ov2640_dev_2;
const struct device *fastvdma_dev_cam_1;
const struct device *fastvdma_dev_cam_2;
const struct device *fastvdma_dev_gpu_in_1;
const struct device *fastvdma_dev_gpu_in_2;
const struct device *fastvdma_dev_gpu_out;
const struct device *gpio_expander;

uint32_t img_buff_1[1280 * 1024];
uint32_t img_buff_2[1280 * 1024];
uint32_t img_buff_3[1280 * 1024];
uint32_t img_buff_4[1280 * 1024];
uint32_t img_buff_5[1280 * 1024];
uint32_t img_buff_6[1280 * 1024];
uint32_t img_buff_7[1280 * 1024];

uint32_t* hdmi_buffers[3] = {img_buff_1, img_buff_2, img_buff_3};
uint32_t* hdmi_buffers_overlay[3] = {img_buff_4, img_buff_5, img_buff_6};

uint32_t image_with_text[1280 * 1024];
uint32_t img_length_1 = 1280 * 1024;
uint32_t img_length_2 = 1280 * 1024;

int cam_buffer_index = 0;
int gpu_buffer_index = 0;

struct video_format fmt_1;
struct video_format fmt_2;
struct video_caps caps;

struct dma_config dma_cfg_cam1 = { 0 };
struct dma_config dma_cfg_cam2 = { 0 };
struct dma_config dma_cfg_gpu_in = { 0 };
struct dma_config dma_cfg_gpu_out = { 0 };
struct dma_block_config dma_block_cfg_cam = { 0 };
struct dma_block_config dma_block_cfg_gpu_in = { 0 };
struct dma_block_config dma_block_cfg_gpu_out = { 0 };

enum mode {cams, overlay } mode;
enum mode mode = cams;

char overlay_text[25] = "Video Overlays 2022";

const k_tid_t hdmi_id;
const k_tid_t cam_id;
const k_tid_t gpu_id;

bool suspend_hdmi = false;
bool suspend_cam = false;
bool suspend_gpu = false;

//define MEASURE_PERFORMANCE

#ifdef MEASURE_PERFORMANCE
timing_t start_time_cam, end_time_cam;
timing_t start_time_gpu, end_time_gpu;
uint64_t measures_cam[100] = {0};
int n_measure_cam = 0;
uint64_t measures_gpu[100] = {0};
int n_measure_gpu = 0;
#endif

struct k_sem my_sem;


K_THREAD_DEFINE(hdmi_id, STACKSIZE, hdmi, NULL, NULL, NULL, PRIORITY, 0, 0);

K_THREAD_DEFINE(cam_id, STACKSIZE, cam, NULL, NULL, NULL, PRIORITY, 0, 0);

K_THREAD_DEFINE(gpu_id, STACKSIZE, gpu, NULL, NULL, NULL, PRIORITY, 0, 0);

void flush_l2_cache(void)
{
#ifdef CONFIG_L2_SIZE
	unsigned int i;
	for (i = 0; i < 2 * CONFIG_L2_SIZE / 4; i++) {
		((volatile unsigned int *)MAIN_RAM_BASE)[i];
	}
#endif
}

void led_chaser()
{
	int pin = 0;
	gpio_port_pins_t mask_high = 0xf;
	gpio_port_pins_t mask_low = 0x0;
	while (true) {
		if (pin == 4) {
			pin = 0;
		}

		mask_high ^= BIT(pin);
		mask_low ^= BIT(pin);

		gpio_port_set_masked(gpio_expander, mask_high, 0xf);
		gpio_port_set_masked(gpio_expander, mask_low, 0x0);

		k_msleep(110);
		pin++;
	}
}

K_THREAD_DEFINE(my_tid, MY_STACK_SIZE, led_chaser, NULL, NULL, NULL, MY_PRIORITY, 0, 0);

void print_camera_caps(const struct device *dev)
{
	if (video_get_caps(dev, VIDEO_EP_OUT, &caps)) {
		printf("Unable to retrieve video capabilities");
		return;
	}

	int i = 0;
	printk("- Capabilities:\n");
	while (caps.format_caps[i].pixelformat) {
		const struct video_format_cap *fcap = &caps.format_caps[i];
		/* fourcc to string */
		printk("  %c%c%c%c width [%u; %u; %u] height [%u; %u; %u]\n",
		       (char)fcap->pixelformat, (char)(fcap->pixelformat >> 8),
		       (char)(fcap->pixelformat >> 16), (char)(fcap->pixelformat >> 24),
		       fcap->width_min, fcap->width_max, fcap->width_step, fcap->height_min,
		       fcap->height_max, fcap->height_step);
		i++;
	}
}

int get_camera_fmt(const struct device *dev, struct video_format *fmt)
{
	/* Get default/native format - camera 1 */
	if (video_get_format(dev, VIDEO_EP_OUT, fmt)) {
		printf("Unable to retrieve video format");
		return -1;
	}

	printf("- Default format: %c%c%c%c %ux%u\n", (char)fmt->pixelformat,
	       (char)(fmt->pixelformat >> 8), (char)(fmt->pixelformat >> 16),
	       (char)(fmt->pixelformat >> 24), fmt->width, fmt->height);

	return 0;
}

void main(void)
{
	k_sem_init(&my_sem, 0, 1);
	suspend_threads();
	
	#ifdef MEASURE_PERFORMANCE
    timing_init();
	timing_start();
	#endif

	ov2640_dev_1 = device_get_binding(OV2640_1);
	ov2640_dev_2 = device_get_binding(OV2640_2);
	fastvdma_dev_cam_1 = device_get_binding(FASTVDMA_1);
	fastvdma_dev_cam_2 = device_get_binding(FASTVDMA_2);
	fastvdma_dev_gpu_in_1 = device_get_binding(FASTVDMA_GPU_IN_1);
	fastvdma_dev_gpu_in_2 = device_get_binding(FASTVDMA_GPU_IN_2);
	fastvdma_dev_gpu_out = device_get_binding(FASTVDMA_GPU_OUT);
	gpio_expander = device_get_binding(GPIO_EXPANDER);

	if (ov2640_dev_1 == NULL || ov2640_dev_2 == NULL) {
		printf("ov2640 camera binding failed.\n");
		return;
	}

	if (fastvdma_dev_cam_1 == NULL || fastvdma_dev_cam_2 == NULL) {
		printf("fastvdma binding failed.\n");
		return;
	}

	if (fastvdma_dev_gpu_in_1 == NULL || fastvdma_dev_gpu_in_2 == NULL ||
	    fastvdma_dev_gpu_out == NULL) {
		printf("GPU fastvdma binding failed.\n");
		return;
	}

	/* Initialize all 5 DMAs */
	dma_init_cam1();
	dma_init_cam2();
	dma_init_gpu_inputs();
	dma_init_gpu_output();


	print_camera_caps(ov2640_dev_1);
	print_camera_caps(ov2640_dev_2);

	get_camera_fmt(ov2640_dev_1, &fmt_1);
	get_camera_fmt(ov2640_dev_2, &fmt_2);

	if (init_socket()) {
		printf("Failed to configure sockets.\n");
		return;
	};
}
