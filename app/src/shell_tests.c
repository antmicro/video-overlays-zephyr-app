/*
 * Copyright (c) 2021-2022 Antmicro
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <device.h>
#include <shell/shell.h>
#include <stdlib.h>
#include <net/socket.h>
#include <drivers/video.h>
#include <drivers/dma.h>
#include <string.h>
#include <timing/timing.h>
#include <stdio.h>

#include "shell_tests.h"
#include "pattern.h"
#include "net.h"
#include "hdmi.h"

void clean_measures(uint64_t measures[])
{
	for (int i = 0; i < 100; i++) {
		measures[i] = 0;
	}
}

void display_measures(uint64_t measures[])
{
	for (int i = 0; i < 100; i++) {
		printf("measure %d : %lld\n", i, measures[i]);
	}
}

static int set_bypass(const struct shell *sh, shell_bypass_cb_t bypass)
{
	static bool in_use;

	if (bypass && in_use) {
		shell_error(sh, "Display supports setting bypass on a single instance");
		return -EBUSY;
	}

	in_use = !in_use;
	if (in_use) {
		shell_print(sh, "press ctrl-x to quit test");
		in_use = true;
	}

	shell_set_bypass(sh, bypass);

	return 0;
}

static void bypass_cb_cams(const struct shell *sh, uint8_t *recv, size_t len)
{
	bool escape = false;

	if (recv[len - 1] == CHAR_CAN) {
		escape = true;
	}

	if (escape) {
		suspend_hdmi = true;
		dma_stop(fastvdma_dev_cam_1, 0);
		dma_stop(fastvdma_dev_cam_2, 0);
		draw_color(fmt_1.width, fmt_1.height, RGB_BLACK);
		k_msleep(10);
		hdmi_out0_core_initiator_base_write((uint32_t)&img_buff_7);
		hdmi_out0_core_initiator_enable_write(1);
#ifdef MEASURE_PERFORMANCE
		printf("Measures cam:\n");
		display_measures(measures_cam);
		clean_measures(measures_cam);
		n_measure_cam = 0;
#endif
		shell_print(sh, "Exiting...");
		set_bypass(sh, NULL);
		return;
	}
}

static void bypass_cb_overlay(const struct shell *sh, uint8_t *recv, size_t len)
{
	bool escape = false;

	if (recv[len - 1] == CHAR_CAN) {
		escape = true;
	}

	if (escape) {
		suspend_gpu = true;
		suspend_cam = true;
		suspend_hdmi = true;
		dma_stop(fastvdma_dev_cam_1, 0);
		dma_stop(fastvdma_dev_cam_2, 0);
		dma_stop(fastvdma_dev_gpu_in_1, 0);
		dma_stop(fastvdma_dev_gpu_in_2, 0);
		dma_stop(fastvdma_dev_gpu_out, 0);
		k_msleep(10);
		draw_color(fmt_1.width, fmt_1.height, RGB_BLACK);
		k_msleep(10);
		hdmi_out0_core_initiator_base_write((uint32_t)&img_buff_7);
		hdmi_out0_core_initiator_enable_write(1);
#ifdef MEASURE_PERFORMANCE
		printf("Measures cam:\n");
		display_measures(measures_cam);
		clean_measures(measures_cam);
		n_measure_cam = 0;
		printf("Measures gpu:\n");
		display_measures(measures_gpu);
		clean_measures(measures_gpu);
		n_measure_gpu = 0;
#endif
		shell_print(sh, "Exiting...");
		k_sem_reset(&my_sem);
		k_sem_give(&my_sem);

		for (int i = 0; i < 3; i++) {
			block_buff[i] = false;
		}

		set_bypass(sh, NULL);
		return;
	}
}

static int cmd_send_image(const struct shell *shell, size_t argc,
				char **argv)
{
	int err = 0;

	if (argc == 1 || (argc == 2 && !strcmp(argv[1], "both"))) {
		shell_print(shell, "ov2640 (both) - send image...");
		send_image((uint32_t *)&img_buff_1, img_length_1);
		send_image((uint32_t *)&img_buff_2, img_length_2);
	} else if (!strcmp(argv[1], "left")) {
		shell_print(shell, "ov2640 (left) - send image...");
		send_image((uint32_t *)&img_buff_1, img_length_1);
	} else if (!strcmp(argv[1], "right")) {
		shell_print(shell, "ov2640 (right) - send image...");
		send_image((uint32_t *)&img_buff_2, img_length_2);
	} else {
		shell_error(shell, "Wrong function parameters.");
		err = -1;
	}

	if (err) {
		shell_error(shell, "ov2640 - send image failed, error: %d", err);
	} else {
		shell_print(shell, "Finished.");
	}

	return err;
}

void blend_images(uint32_t read_addr, uint32_t write_addr)
{
	/* Configure DMAs */
	dma_block_cfg_gpu_in.source_address = read_addr;
	dma_block_cfg_gpu_in.dest_address = 0;
	dma_config(fastvdma_dev_gpu_in_2, 0, &dma_cfg_gpu_in);
	dma_block_cfg_gpu_out.source_address = 0;
	dma_block_cfg_gpu_out.dest_address = write_addr;
	dma_config(fastvdma_dev_gpu_out, 0, &dma_cfg_gpu_out);

	/* Start GPU DMAs */
	dma_start(fastvdma_dev_gpu_in_1, 0);
	dma_start(fastvdma_dev_gpu_in_2, 0);
#ifdef MEASURE_PERFORMANCE
	start_time_gpu = timing_counter_get();
#endif
	dma_start(fastvdma_dev_gpu_out, 0);
}

static int cmd_display_colors(const struct shell *shell, size_t argc, char **argv)
{
	hdmi_out0_core_initiator_enable_write(1);
	draw_color(fmt_1.height, fmt_1.width, RGB_RED);
	k_msleep(1000);
	hdmi_out0_core_initiator_base_write((uint32_t)&img_buff_7);

	k_msleep(1000);
	draw_color(fmt_1.height, fmt_1.width, RGB_GREEN);
	k_msleep(1000);
	hdmi_out0_core_initiator_base_write((uint32_t)&img_buff_7);

	k_msleep(1000);
	draw_color(fmt_1.height, fmt_1.width, RGB_BLUE);
	k_msleep(1000);
	hdmi_out0_core_initiator_base_write((uint32_t)&img_buff_7);

	return 0;
}

void display_buffer1(void)
{
	/* disable loop mode */
	dma_block_cfg_cam.source_gather_en = 0;
	dma_block_cfg_cam.dest_scatter_en = 0;
	dma_cfg_cam1.dma_callback = NULL;
	dma_block_cfg_cam.dest_address = (uint32_t)&img_buff_1;

	dma_config(fastvdma_dev_cam_1, 0, &dma_cfg_cam1);
	dma_start(fastvdma_dev_cam_1, 0);
	hdmi_out0_core_initiator_enable_write(1);
	hdmi_out0_core_initiator_base_write((uint32_t)&img_buff_1);

	/* enable loop mode */
	dma_block_cfg_cam.source_gather_en = 1;
	dma_block_cfg_cam.dest_scatter_en = 1;
}

void display_buffer2(void)
{
	/* disable loop mode */
	dma_block_cfg_cam.source_gather_en = 0;
	dma_block_cfg_cam.dest_scatter_en = 0;
	dma_cfg_cam2.dma_callback = NULL;
	dma_block_cfg_cam.dest_address = (uint32_t)&img_buff_1;

	dma_config(fastvdma_dev_cam_2, 0, &dma_cfg_cam2);
	dma_start(fastvdma_dev_cam_2, 0);
	hdmi_out0_core_initiator_enable_write(1);
	hdmi_out0_core_initiator_base_write((uint32_t)&img_buff_1);

	/* enable loop mode */
	dma_block_cfg_cam.source_gather_en = 1;
	dma_block_cfg_cam.dest_scatter_en = 1;
}

void display_video_cam1(void)
{
	suspend_hdmi = false;

	dma_cfg_cam1.dma_callback = cam1_dma_user_callback;
	dma_config(fastvdma_dev_cam_1, 0, &dma_cfg_cam1);
#ifdef MEASURE_PERFORMANCE
	start_time_cam = timing_counter_get();
#endif
	dma_start(fastvdma_dev_cam_1, 0);
	mode = cams;
	k_thread_resume(hdmi_id);
}

void display_video_cam2(void)
{
	suspend_hdmi = false;

	dma_cfg_cam2.dma_callback = cam2_dma_user_callback;
	dma_config(fastvdma_dev_cam_2, 0, &dma_cfg_cam2);
#ifdef MEASURE_PERFORMANCE
	start_time_cam = timing_counter_get();
#endif
	dma_start(fastvdma_dev_cam_2, 0);
	mode = cams;
	k_thread_resume(hdmi_id);
}

void display_video_with_overlay_cam1(void)
{
	suspend_gpu = false;
	suspend_cam = false;
	suspend_hdmi = false;

	dma_cfg_cam1.dma_callback = cam_with_gpu_dma_user_callback;
	dma_block_cfg_cam.dest_address = (uint32_t)&img_buff_1;
	dma_config(fastvdma_dev_cam_1, 0, &dma_cfg_cam1);

	dma_block_cfg_gpu_in.dest_address = 0;
	dma_block_cfg_gpu_in.source_gather_count = fmt_1.height;
	dma_block_cfg_gpu_in.dest_scatter_count = fmt_1.height;
	dma_block_cfg_gpu_in.block_size = fmt_1.width * fmt_1.height;
	dma_block_cfg_gpu_out.source_gather_count = fmt_1.height;
	dma_block_cfg_gpu_out.dest_scatter_count = fmt_1.height;
	dma_block_cfg_gpu_out.block_size = fmt_1.width * fmt_1.height;
	dma_config(fastvdma_dev_gpu_in_1, 0, &dma_cfg_gpu_in);
	dma_config(fastvdma_dev_gpu_in_2, 0, &dma_cfg_gpu_in);
	dma_config(fastvdma_dev_gpu_out, 0, &dma_cfg_gpu_out);

	/* Start GPU */
	GPU->CR = GPU_CR_ALPHA_BLENDER;

	blocked_buff_cam = 0;
	blocked_buff_gpu = 1;
	block_buff[blocked_buff_cam] = true;
	block_buff[blocked_buff_gpu] = true;

#ifdef MEASURE_PERFORMANCE
	start_time_cam = timing_counter_get();
#endif

	dma_start(fastvdma_dev_cam_1, 0);

	mode = overlay;
	k_thread_resume(hdmi_id);
	k_thread_resume(cam_id);
	k_thread_resume(gpu_id);

#ifdef MEASURE_PERFORMANCE
	start_time_gpu = timing_counter_get();
#endif
	blend_images((uint32_t)&img_buff_1, (uint32_t)&img_buff_4);
}

void display_video_with_overlay_cam2(void)
{
	suspend_gpu = false;
	suspend_cam = false;
	suspend_hdmi = false;

	dma_cfg_cam2.dma_callback = cam_with_gpu_dma_user_callback;
	dma_block_cfg_cam.dest_address = (uint32_t)&img_buff_1;
	dma_config(fastvdma_dev_cam_2, 0, &dma_cfg_cam2);

	dma_block_cfg_gpu_in.dest_address = 0;
	dma_block_cfg_gpu_in.source_gather_count = fmt_1.height;
	dma_block_cfg_gpu_in.dest_scatter_count = fmt_1.height;
	dma_block_cfg_gpu_in.block_size = fmt_1.width * fmt_1.height;
	dma_block_cfg_gpu_out.source_gather_count = fmt_1.height;
	dma_block_cfg_gpu_out.dest_scatter_count = fmt_1.height;
	dma_block_cfg_gpu_out.block_size = fmt_1.width * fmt_1.height;
	dma_config(fastvdma_dev_gpu_in_1, 0, &dma_cfg_gpu_in);
	dma_config(fastvdma_dev_gpu_in_2, 0, &dma_cfg_gpu_in);
	dma_config(fastvdma_dev_gpu_out, 0, &dma_cfg_gpu_out);

	/* Start GPU */
	GPU->CR = GPU_CR_ALPHA_BLENDER;

	blocked_buff_cam = 0;
	blocked_buff_gpu = 1;
	block_buff[blocked_buff_cam] = true;
	block_buff[blocked_buff_gpu] = true;
#ifdef MEASURE_PERFORMANCE
	start_time_cam = timing_counter_get();
#endif

	dma_start(fastvdma_dev_cam_2, 0);

	mode = overlay;
	k_thread_resume(hdmi_id);
	k_thread_resume(cam_id);
	k_thread_resume(gpu_id);
#ifdef MEASURE_PERFORMANCE
	start_time_gpu = timing_counter_get();
#endif
	blend_images((uint32_t)&img_buff_1, (uint32_t)&img_buff_4);
}

static int cmd_display_image_with_overlay(const struct shell *shell, size_t argc, char **argv)
{
	hdmi_out0_core_initiator_enable_write(1);
	dma_cfg_cam1.dma_callback = cam1_dma_user_callback;
	dma_block_cfg_cam.dest_address = (uint32_t)&img_buff_1;
	dma_config(fastvdma_dev_cam_1, 0, &dma_cfg_cam1);
	dma_start(fastvdma_dev_cam_1, 0);
	k_msleep(300);
	dma_stop(fastvdma_dev_cam_1, 0);

	if (overlay_mode == text) {
		generate_image_with_text(overlay_image, fmt_1.width, fmt_1.height);
	} else {
		if(generate_image_with_logo(overlay_image, fmt_1.width, fmt_1.height, logo_offset_x, logo_offset_y)) {
			shell_error(shell, "Logo placed out of image border, did not generate.");
		}
	}

	blend_images((uint32_t)&img_buff_1, (uint32_t)&img_buff_7);

	hdmi_out0_core_initiator_base_write((uint32_t)&img_buff_7);

	return 0;
}

static int cmd_generate_and_send_overlay(const struct shell *shell, size_t argc,
				char **argv)
{
	if (overlay_mode == text) {
		generate_image_with_text(overlay_image, fmt_1.width, fmt_1.height);
	} else {
		if(generate_image_with_logo(overlay_image, fmt_1.width, fmt_1.height, logo_offset_x, logo_offset_y)) {
			shell_error(shell, "Logo placed out of image border, did not generate.");
		}
	}

	shell_print(shell, "send generated image...");
	send_image((uint32_t *)&overlay_image, img_length_1);

	return 0;
}

static int cmd_display_buffer(const struct shell *shell, size_t argc, char **argv)
{
	if (!strcmp(argv[1], "left")) {
		shell_print(shell, "Displaying single buffer of LEFT camera...");
		display_buffer1();
	} else if (!strcmp(argv[1], "right")) {
		shell_print(shell, "Displaying single buffer of RIGHT camera...");
		display_buffer2();
	} else {
		shell_error(shell, "Wrong function parameters.");
	}

	return 0;
}

static int cmd_display_video(const struct shell *shell, size_t argc, char **argv)
{
	if (!strcmp(argv[1], "left")) {
		shell_print(shell, "Displaying stream from LEFT camera...");
		display_video_cam1();
		set_bypass(shell, bypass_cb_cams);
	} else if (!strcmp(argv[1], "right")) {
		shell_print(shell, "Displaying stream from RIGHT camera...");
		display_video_cam2();
		set_bypass(shell, bypass_cb_cams);
	} else {
		shell_error(shell, "Wrong function parameters.");
	}

	return 0;
}

static int cmd_display_video_with_overlay(const struct shell *shell, size_t argc, char **argv)
{
	if (!strcmp(argv[1], "left")) {
		shell_print(shell, "Displaying stream from LEFT camera with applied overlay...");

		if (overlay_mode == text) {
			generate_image_with_text(overlay_image, fmt_1.width, fmt_1.height);
		} else {
			if(generate_image_with_logo(overlay_image, fmt_1.width, fmt_1.height, logo_offset_x, logo_offset_y)) {
				shell_error(shell, "Logo placed out of image border, did not generate.");
			}
		}

		dma_block_cfg_gpu_in.source_address = (uint32_t)&overlay_image;
		display_video_with_overlay_cam1();
		set_bypass(shell, bypass_cb_overlay);
	} else if (!strcmp(argv[1], "right")) {
		shell_print(shell, "Displaying stream from RIGHT camera with applied overlay...");

		if (overlay_mode == text) {
			generate_image_with_text(overlay_image, fmt_2.width, fmt_2.height);
		} else {
			if(generate_image_with_logo(overlay_image, fmt_2.width, fmt_2.height, logo_offset_x, logo_offset_y)) {
				shell_error(shell, "Logo placed out of image border, did not generate.");
			}
		}

		dma_block_cfg_gpu_in.source_address = (uint32_t)&overlay_image;
		display_video_with_overlay_cam2();
		set_bypass(shell, bypass_cb_overlay);
	} else {
		shell_error(shell, "Wrong function parameters.");
	}

	return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(
	sub_test,
	SHELL_CMD_ARG(display_colors, NULL,
		"\tDisplay red, green and blue image in 800x600 resolution with 2sdelay between each other. It DOES NOT change HDMI resolution.",
		cmd_display_colors, 1, 0),
	SHELL_CMD_ARG(display_buffer, NULL,
		"\tDisplay content of camera buffer. Choose between 'left' or 'right' camera.It DOES NOT change HDMI resolution.",
		cmd_display_buffer, 2, 0),
	SHELL_CMD_ARG(display_video, NULL,
		"\tDisplay images captured by choosed camera as a stream. Choose between 'left' or 'right' camera. It DOES NOT change HDMI resolution.",
		cmd_display_video, 2, 0),
	SHELL_CMD_ARG(display_video_with_overlay, NULL,
		"\tDisplay images captured by choosed camera as a stream with an overlay applied on themIt DOES NOT change HDMI resolution.",
		cmd_display_video_with_overlay, 2, 0),
	SHELL_CMD_ARG(display_image_with_overlay, NULL,
		"\tDisplay single picture captured by left camera with overlaye applied on top of it.It DOES NOT change HDMI resolution.",
		cmd_display_image_with_overlay, 1, 0),
	SHELL_CMD_ARG(generate_and_send_overlay, NULL,
		"\tGenerate an overlay image. Text is fixed to \"2021-10-27 9:31\"After successful generation it is sent via UDP at 192.0.2.2.It DOES NOT display generated image.",
		cmd_generate_and_send_overlay, 1, 0),
	SHELL_CMD_ARG(send_image, NULL,
		"\tSend image via UPD at 192.0.2.2. Provide either left or right to choose which buffer should be sent. If none provided these will be sent one after another starting with the left one.\n",
	cmd_send_image, 1, 1),
	SHELL_SUBCMD_SET_END);

SHELL_CMD_REGISTER(test, &sub_test, "\tTest app functionalities.", NULL);
