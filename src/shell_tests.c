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
#include "hdmi.h"

void blend_images(uint32_t read_addr1, uint32_t read_addr2, uint32_t write_addr)
{
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

static int display_colors(const struct shell *shell, size_t argc, char **argv)
{
	hdmi_out0_core_initiator_enable_write(1);
	draw_color(800, 600, RGB_RED);
	k_msleep(1000);
	hdmi_out0_core_initiator_base_write(img_buff_10);

	k_msleep(1000);
	draw_color(800, 600, RGB_GREEN);
	k_msleep(1000);
	hdmi_out0_core_initiator_base_write(img_buff_10);

	k_msleep(1000);
	draw_color(800, 600, RGB_BLUE);
	k_msleep(1000);
	hdmi_out0_core_initiator_base_write(img_buff_10);

    return 0;
}

static int display_buffer1(const struct shell *shell, size_t argc, char **argv)
{
	hdmi_out0_core_initiator_enable_write(1);
	shell_print(shell, "Displaying image from camera 1");
	hdmi_out0_core_initiator_base_write((uint32_t)&img_buff_1);

	return 0;
}

static int display_buffer2(const struct shell *shell, size_t argc, char **argv)
{
	hdmi_out0_core_initiator_enable_write(1);
	shell_print(shell, "Displaying image from camera 2");
	hdmi_out0_core_initiator_base_write((uint32_t)&img_buff_2);

	return 0;
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
		shell_print(sh, "Displaying...\npress ctrl-x escape");
		in_use = true;
	}

	shell_set_bypass(sh, bypass);

	return 0;
}

static void bypass_cb(const struct shell *sh, uint8_t *recv, size_t len)
{
	bool escape = false;

	if (recv[len - 1] == CHAR_CAN ) {
		escape = true;
	} 

	if (escape) {
		k_thread_suspend(hdmi_id);
		draw_color(800, 600, RGB_BLACK);
		k_msleep(10);
		hdmi_out0_core_initiator_base_write(img_buff_10);
		hdmi_out0_core_initiator_enable_write(1);
		shell_print(sh, "Exiting...");
		set_bypass(sh, NULL);
		return;
	}
}

static int display_video_cam1(const struct shell *shell, size_t argc, char **argv)
{
	dma_cfg_cam1.dma_callback = cam1_dma_user_callback;
	dma_config(fastvdma_dev_cam_1, 0, &dma_cfg_cam1);
	dma_stop(fastvdma_dev_cam_2, 0);
	dma_start(fastvdma_dev_cam_1, 0);
	mode = cam1;
	k_thread_resume(hdmi_id);
	set_bypass(shell, bypass_cb);
	return 0;
}

static int display_video_cam2(const struct shell *shell, size_t argc, char **argv)
{
	dma_cfg_cam2.dma_callback = cam2_dma_user_callback;
	dma_config(fastvdma_dev_cam_2, 0, &dma_cfg_cam2);
	dma_stop(fastvdma_dev_cam_1, 0);
	dma_start(fastvdma_dev_cam_2, 0);
	mode = cam2;
	k_thread_resume(hdmi_id);
	set_bypass(shell, bypass_cb);
	return 0;
}

static int display_video_with_overlay_cam1(const struct shell *shell, size_t argc, char **argv)
{
	dma_cfg_cam1.dma_callback = cam1_with_gpu_dma_user_callback;
	dma_config(fastvdma_dev_cam_1, 0, &dma_cfg_cam1);
	dma_stop(fastvdma_dev_cam_2, 0);
	dma_start(fastvdma_dev_cam_1, 0);
	
	char *text = "2021-11-25 10:00";
	generate_image_with_text(&image_with_text, text, fmt_1.width, fmt_1.height);
	blend_images(image_with_text, img_buff_1, img_buff_2);

	mode = overlay;
	k_thread_resume(hdmi_id);
	set_bypass(shell, bypass_cb);
	return 0;
}

static int display_video_with_overlay_cam2(const struct shell *shell, size_t argc, char **argv)
{
	dma_cfg_cam2.dma_callback = cam2_with_gpu_dma_user_callback;
	dma_config(fastvdma_dev_cam_2, 0, &dma_cfg_cam2);
	dma_stop(fastvdma_dev_cam_1, 0);
	dma_start(fastvdma_dev_cam_2, 0);
	
	char *text = "2021-11-25 10:00";
	generate_image_with_text(&image_with_text, text, fmt_1.width, fmt_1.height);
	blend_images(image_with_text, img_buff_1, img_buff_2);

	mode = overlay;
	k_thread_resume(hdmi_id);
	set_bypass(shell, bypass_cb);
	return 0;
}

static int display_video_with_overlay(const struct shell *shell, size_t argc, char **argv)
{
	hdmi_out0_core_initiator_enable_write(1);

	flush_l2_cache();
	dma_block_cfg_cam.dest_address = (uint32_t)&img_buff_2;
	dma_config(fastvdma_dev_cam_1, 0, &dma_cfg_cam1);
	dma_start(fastvdma_dev_cam_1, 0);
	struct dma_status stat;
	char *text = "2021-11-25 10:00";

	generate_image_with_text((uint32_t*)&image_with_text, text, fmt_1.width, fmt_1.height);
	k_msleep(100);
	blend_images((uint32_t)image_with_text, (uint32_t)img_buff_2, (uint32_t)img_buff_4);
	k_msleep(100);

	while(1){
		flush_l2_cache();
		dma_block_cfg_cam.source_address = 0;
		dma_block_cfg_cam.dest_address = (uint32_t)&img_buff_1;
		dma_config(fastvdma_dev_cam_1, 0, &dma_cfg_cam1);
		dma_start(fastvdma_dev_cam_1, 0);
		k_msleep(10);
		dma_get_status(fastvdma_dev_cam_1, 0, &stat);
		while(stat.busy != 0){
			dma_get_status(fastvdma_dev_cam_1, 0, &stat);
		}
		k_msleep(100);
		blend_images((uint32_t)image_with_text, (uint32_t)img_buff_1, (uint32_t)img_buff_5);
		k_msleep(100);
		hdmi_out0_core_initiator_base_write((uint32_t)&img_buff_4);
		k_msleep(10);
		flush_l2_cache();
		dma_block_cfg_cam.source_address = 0;
		dma_block_cfg_cam.dest_address = (uint32_t)&img_buff_3;
		dma_config(fastvdma_dev_cam_1, 0, &dma_cfg_cam1);
		dma_start(fastvdma_dev_cam_1, 0);
		k_msleep(10);
		dma_get_status(fastvdma_dev_cam_1, 0, &stat);
		while(stat.busy != 0){
			dma_get_status(fastvdma_dev_cam_1, 0, &stat);
		}
		k_msleep(100);
		blend_images((uint32_t)image_with_text, (uint32_t)img_buff_3, (uint32_t)img_buff_6);
		k_msleep(100);
		hdmi_out0_core_initiator_base_write((uint32_t)&img_buff_5);
		k_msleep(10);
		flush_l2_cache();
		dma_block_cfg_cam.dest_address = (uint32_t)&img_buff_2;
		dma_config(fastvdma_dev_cam_1, 0, &dma_cfg_cam1);
		dma_start(fastvdma_dev_cam_1, 0);
		k_msleep(10);
	    dma_get_status(fastvdma_dev_cam_1, 0, &stat);
		while(stat.busy != 0){
			dma_get_status(fastvdma_dev_cam_1, 0, &stat);
		}
		k_msleep(100);
		blend_images((uint32_t)image_with_text, (uint32_t)img_buff_2, (uint32_t)img_buff_4);
		k_msleep(100);
		hdmi_out0_core_initiator_base_write((uint32_t)&img_buff_6);
		k_msleep(10);
	}

	return 0;
}

static int display_image_with_overlay(const struct shell *shell, size_t argc, char **argv)
{
	char *text = "2021-11-25 10:00";

	hdmi_out0_core_initiator_enable_write(1);
	flush_l2_cache();

	dma_block_cfg_cam.dest_address = (uint32_t)&img_buff_1;
	dma_config(fastvdma_dev_cam_1, 0, &dma_cfg_cam1);
	dma_start(fastvdma_dev_cam_1, 0);

	generate_image_with_text((uint32_t*)&image_with_text, text, fmt_1.width, fmt_1.height);
	blend_images((uint32_t)image_with_text, (uint32_t)img_buff_1, (uint32_t)img_buff_2);

	hdmi_out0_core_initiator_base_write((uint32_t)&img_buff_2);

	return 0;
}

static int cmd_generate_and_send_overlay(const struct shell *shell, size_t argc,
				char **argv)
{
	char *text = "2021-10-27 9:31";

	generate_image_with_text((uint32_t*)&image_with_text, text, fmt_1.width, fmt_1.height);

	shell_print(shell, "send generated image...");
	send_image((uint32_t*)&image_with_text, img_length_1);

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
	SHELL_CMD_ARG(display_video_cam1, NULL,
		"\tDisplay pictures captured by left camera in continous mode."
		"It DOES NOT change HDMI resolution."
		"WARNING: This test is blocking, you can stop this after invoke!", display_video_cam1, 1, 0),
	SHELL_CMD_ARG(display_video_cam2, NULL,
		"\tDisplay pictures captured by right camera in continous mode."
		"It DOES NOT change HDMI resolution."
		"WARNING: This test is blocking, you can stop this after invoke!", display_video_cam2, 1, 0),
	SHELL_CMD_ARG(display_video_with_overlay_cam1, NULL,
		"\tDisplay pictures captured by left camera with overlayed applied on them"
		"in continous mode. It DOES NOT change HDMI resolution."
		"WARNING: This test is blocking, you can stop this after invoke!", display_video_with_overlay_cam1, 1, 0),
	SHELL_CMD_ARG(display_video_with_overlay_cam2, NULL,
		"\tDisplay pictures captured by right camera with overlayed applied on them"
		"in continous mode. It DOES NOT change HDMI resolution."
		"WARNING: This test is blocking, you can stop this after invoke!", display_video_with_overlay_cam2, 1, 0),
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
