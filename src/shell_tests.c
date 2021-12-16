#include <zephyr.h>
#include <device.h>
#include <shell/shell.h>
#include <stdlib.h>
#include <net/socket.h>
#include <drivers/video.h>
#include <drivers/dma.h>
#include <string.h>

#include "shell_tests.h"
#include "pattern.h"
#include "net.h"

static int display_colors(const struct shell *shell, size_t argc, char **argv)
{
	hdmi_out0_core_initiator_enable_write(1);
	draw_color(800, 600, RGB_RED);
	k_msleep(1000);
	hdmi_out0_core_initiator_base_write(BUFFER_ADDR);

	k_msleep(1000);
	draw_color(800, 600, RGB_GREEN);
	k_msleep(1000);
	hdmi_out0_core_initiator_base_write(BUFFER_ADDR);

	k_msleep(1000);
	draw_color(800, 600, RGB_BLUE);
	k_msleep(1000);
	hdmi_out0_core_initiator_base_write(BUFFER_ADDR);

    return 0;
}

static int display_buffer1(const struct shell *shell, size_t argc, char **argv)
{
	hdmi_out0_core_initiator_enable_write(1);
	shell_print(shell, "Displaying image from camera 1");
	hdmi_out0_core_initiator_base_write(&img_buff_1);
}

static int display_buffer2(const struct shell *shell, size_t argc, char **argv)
{
	hdmi_out0_core_initiator_enable_write(1);
	shell_print(shell, "Displaying image from camera 2");
	hdmi_out0_core_initiator_base_write(&img_buff_2);
}

static int display_video(const struct shell *shell, size_t argc, char **argv)
{
	hdmi_out0_core_initiator_enable_write(1);

	flush_l2_cache();
	dma_block_cfg_cam.dest_address = &img_buff_2;
	dma_config(fastvdma_dev_cam_1, 0, &dma_cfg_cam);
	dma_start(fastvdma_dev_cam_1, 0);
	struct dma_status stat;

	while(1){
		flush_l2_cache();
		dma_block_cfg_cam.dest_address = &img_buff_1;
		dma_config(fastvdma_dev_cam_1, 0, &dma_cfg_cam);
		dma_start(fastvdma_dev_cam_1, 0);
		k_msleep(10);
		dma_get_status(fastvdma_dev_cam_1, 0, &stat);
		while(stat.busy != 0){
			dma_get_status(fastvdma_dev_cam_1, 0, &stat);
		}
		hdmi_out0_core_initiator_base_write(&img_buff_2);
		k_msleep(10);
		flush_l2_cache();
		dma_block_cfg_cam.dest_address = &img_buff_3;
		dma_config(fastvdma_dev_cam_1, 0, &dma_cfg_cam);
		dma_start(fastvdma_dev_cam_1, 0);
		k_msleep(10);
		dma_get_status(fastvdma_dev_cam_1, 0, &stat);
		while(stat.busy != 0){
			dma_get_status(fastvdma_dev_cam_1, 0, &stat);
		}
		hdmi_out0_core_initiator_base_write(&img_buff_1);
		k_msleep(10);
		flush_l2_cache();
		dma_block_cfg_cam.dest_address = &img_buff_2;
		dma_config(fastvdma_dev_cam_1, 0, &dma_cfg_cam);
		dma_start(fastvdma_dev_cam_1, 0);
		k_msleep(10);
	    dma_get_status(fastvdma_dev_cam_1, 0, &stat);
		while(stat.busy != 0){
			dma_get_status(fastvdma_dev_cam_1, 0, &stat);
		}
		hdmi_out0_core_initiator_base_write(&img_buff_3);
		k_msleep(10);
	}
}

static int display_video_with_overlay(const struct shell *shell, size_t argc, char **argv)
{
	hdmi_out0_core_initiator_enable_write(1);

	flush_l2_cache();
	dma_block_cfg_cam.dest_address = &img_buff_2;
	dma_config(fastvdma_dev_cam_1, 0, &dma_cfg_cam);
	dma_start(fastvdma_dev_cam_1, 0);
	struct dma_status stat;
	char *text = "2021-11-25 10:00";

	generate_image_with_text(&image_with_text, text, fmt_1.width, fmt_1.height);
	k_msleep(100);
	blend_images(image_with_text, img_buff_2, img_buff_4);
	k_msleep(100);

	while(1){
		flush_l2_cache();
		dma_block_cfg_cam.source_address = 0;
		dma_block_cfg_cam.dest_address = &img_buff_1;
		dma_config(fastvdma_dev_cam_1, 0, &dma_cfg_cam);
		dma_start(fastvdma_dev_cam_1, 0);
		k_msleep(10);
		dma_get_status(fastvdma_dev_cam_1, 0, &stat);
		while(stat.busy != 0){
			dma_get_status(fastvdma_dev_cam_1, 0, &stat);
		}
		k_msleep(100);
		blend_images(image_with_text, img_buff_1, img_buff_5);
		k_msleep(100);
		hdmi_out0_core_initiator_base_write(&img_buff_4);
		k_msleep(10);
		flush_l2_cache();
		dma_block_cfg_cam.source_address = 0;
		dma_block_cfg_cam.dest_address = &img_buff_3;
		dma_config(fastvdma_dev_cam_1, 0, &dma_cfg_cam);
		dma_start(fastvdma_dev_cam_1, 0);
		k_msleep(10);
		dma_get_status(fastvdma_dev_cam_1, 0, &stat);
		while(stat.busy != 0){
			dma_get_status(fastvdma_dev_cam_1, 0, &stat);
		}
		k_msleep(100);
		blend_images(image_with_text, img_buff_3, img_buff_6);
		k_msleep(100);
		hdmi_out0_core_initiator_base_write(&img_buff_5);
		k_msleep(10);
		flush_l2_cache();
		dma_block_cfg_cam.dest_address = &img_buff_2;
		dma_config(fastvdma_dev_cam_1, 0, &dma_cfg_cam);
		dma_start(fastvdma_dev_cam_1, 0);
		k_msleep(10);
	    dma_get_status(fastvdma_dev_cam_1, 0, &stat);
		while(stat.busy != 0){
			dma_get_status(fastvdma_dev_cam_1, 0, &stat);
		}
		k_msleep(100);
		blend_images(image_with_text, img_buff_2, img_buff_4);
		k_msleep(100);
		hdmi_out0_core_initiator_base_write(&img_buff_6);
		k_msleep(10);
	}
}

static int display_image_with_overlay(const struct shell *shell, size_t argc, char **argv)
{
	hdmi_out0_core_initiator_enable_write(1);

	flush_l2_cache();
	dma_block_cfg_cam.dest_address = &img_buff_1;
	dma_config(fastvdma_dev_cam_1, 0, &dma_cfg_cam);
	dma_start(fastvdma_dev_cam_1, 0);
	struct dma_status stat;
	char *text = "2021-11-25 10:00";
	generate_image_with_text(&image_with_text, text, fmt_1.width, fmt_1.height);
	blend_images(image_with_text, img_buff_1, img_buff_2);
	hdmi_out0_core_initiator_base_write(&img_buff_2);
}

void blend_images(uint32_t read_addr1, uint32_t read_addr2, uint32_t write_addr)
{
	struct dma_status status_gpu_in_1;
	struct dma_status status_gpu_in_2;
	struct dma_status status_gpu_out;

	/* Configure DMAs */
	dma_block_cfg_gpu_in.source_address = read_addr1;
	dma_block_cfg_gpu_in.dest_address = 0;
	dma_config(fastvdma_dev_gpu_in_1, 0, &dma_cfg_gpu_in);
	dma_block_cfg_gpu_in.source_address = read_addr2;
	dma_block_cfg_gpu_in.dest_address = 0;
	dma_config(fastvdma_dev_gpu_in_2, 0, &dma_cfg_gpu_in);
	dma_block_cfg_gpu_out.source_address = 0;
	dma_block_cfg_gpu_out.dest_address = write_addr;
	dma_config(fastvdma_dev_gpu_out, 0, &dma_cfg_gpu_out);

	/* Start GPU */
	GPU->CR = GPU_CR_ALPHA_BLENDER;

	/* Start GPU DMAs */
	dma_start(fastvdma_dev_gpu_in_1, 0);
	dma_start(fastvdma_dev_gpu_in_2, 0);
	dma_start(fastvdma_dev_gpu_out, 0);
}

static int cmd_generate_and_send_overlay(const struct shell *shell, size_t argc,
				char **argv)
{
	char *text = "2021-10-27 9:31";

	generate_image_with_text(&image_with_text, text, fmt_1.width, fmt_1.height);

	shell_print(shell, "send generated image...");
	send_image(&image_with_text, img_length_1);

	return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(
	sub_test,
	SHELL_CMD_ARG(display_colors, NULL,
		"\tDisplay red, green and blue image in 800x600 resolution with 2s"
		"delay between each other. It DOES NOT change HDMI resolution.", display_colors, 1, 0),
	SHELL_CMD_ARG(display_buffer1, NULL,
		"\tDisplay content of buffer1."
		"It DOES NOT change HDMI resolution.", display_buffer1, 1, 0),
	SHELL_CMD_ARG(display_buffer2, NULL,
		"\tDisplay content of buffer2."
		"It DOES NOT change HDMI resolution.", display_buffer2, 1, 0),
	SHELL_CMD_ARG(display_video, NULL,
		"\tDisplay pictures captured by left camera in continous mode."
		"It DOES NOT change HDMI resolution."
		"WARNING: This test is blocking, you can stop this after invoke!", display_video, 1, 0),
	SHELL_CMD_ARG(display_video_with_overlay, NULL,
		"\tDisplay pictures captured by left camera with overlayed applied on them"
		"in continous mode. It DOES NOT change HDMI resolution."
		"WARNING: This test is blocking, you can stop this after invoke!", display_video_with_overlay, 1, 0),
	SHELL_CMD_ARG(display_image_with_overlay, NULL,
		"\tDisplay single picture captured by left camera with overlaye applied on top of it."
		"It DOES NOT change HDMI resolution.", display_image_with_overlay, 1, 0),
	SHELL_CMD_ARG(generate_and_send_overlay, NULL,
		"\tGenerate an overlay image. Text is fixed to \"2021-10-27 9:31\""
		"After successful generation it is sent via UDP at 192.0.2.2."
		"It DOES NOT display generated image.", cmd_generate_and_send_overlay, 1, 0),
	SHELL_SUBCMD_SET_END);

SHELL_CMD_REGISTER(test, &sub_test, "\tTest app functionalities.", NULL);
