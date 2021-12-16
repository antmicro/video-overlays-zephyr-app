#include <zephyr.h>
#include <device.h>
#include <shell/shell.h>
#include <stdlib.h>
#include <drivers/video.h>
#include <drivers/dma.h>
#include <string.h>
#include <net/socket.h>

#include "net.h"
#include "shell_tests.h"

static int cmd_ov2640_set_brightness(const struct shell *shell, size_t argc,
			      char **argv)
{
	int err = 0;
    int arg = atoi(argv[1]);

	if (argc == 2 || (argc == 3 && !strcmp(argv[2], "both"))) {
		shell_print(shell, "ov2640 (both) - set brightness to %s...", argv[1]);
		err |= video_set_ctrl(ov2640_dev_1,
			VIDEO_CID_CAMERA_BRIGHTNESS, &arg);
		err |= video_set_ctrl(ov2640_dev_2,
			VIDEO_CID_CAMERA_BRIGHTNESS, &arg);	
	} else if (!strcmp(argv[2], "left")) {
		shell_print(shell, "ov2640 (left) - set brightness to %s...", argv[1]);
		err |= video_set_ctrl(ov2640_dev_1,
			VIDEO_CID_CAMERA_BRIGHTNESS, &arg);
	} else if (!strcmp(argv[2], "right")) {
		shell_print(shell, "ov2640 (right) - set brightness to %s...", argv[1]);
		err |= video_set_ctrl(ov2640_dev_2,
			VIDEO_CID_CAMERA_BRIGHTNESS, &arg);
	} else {
		shell_error(shell, "Wrong function parameters.");
	}

	if (err) {
		shell_error(shell, "ov2640 - set brightness failed, error: %d", err);
	}

	shell_print(shell, "Finished.");

	return err;
}

static int cmd_ov2640_set_saturation(const struct shell *shell, size_t argc,
			      char **argv)
{
	int err = 0;
    int arg = atoi(argv[1]);

	if (argc == 2 || (argc == 3 && !strcmp(argv[2], "both"))) {
		shell_print(shell, "ov2640 (both) - set saturation to %s...", argv[1]);
		err |= video_set_ctrl(ov2640_dev_1,
			VIDEO_CID_CAMERA_SATURATION, &arg);
		err |= video_set_ctrl(ov2640_dev_2,
			VIDEO_CID_CAMERA_SATURATION, &arg);	
	} else if (!strcmp(argv[2], "left")) {
		shell_print(shell, "ov2640 (left) - set saturation to %s...", argv[1]);
		err |= video_set_ctrl(ov2640_dev_1,
			VIDEO_CID_CAMERA_SATURATION, &arg);
	} else if (!strcmp(argv[2], "right")) {
		shell_print(shell, "ov2640 (right) - set saturation to %s...", argv[1]);
		err |= video_set_ctrl(ov2640_dev_2,
			VIDEO_CID_CAMERA_SATURATION, &arg);
	} else {
		shell_error(shell, "Wrong function parameters.");
	}

	if (err) {
		shell_error(shell, "ov2640 - set saturation failed, error: %d", err);
	}

	shell_print(shell, "Finished.");

	return err;
}

static int cmd_ov2640_set_output_format(const struct shell *shell, size_t argc,
				 char **argv)
{
	int err = 0;

	if (strcmp(argv[1], "JPEG")) {
		fmt_1.pixelformat = VIDEO_PIX_FMT_JPEG;
		fmt_2.pixelformat = VIDEO_PIX_FMT_JPEG;
	} else if (strcmp(argv[1], "RGB565")) {
		fmt_1.pixelformat = VIDEO_PIX_FMT_RGB565;
		fmt_2.pixelformat = VIDEO_PIX_FMT_RGB565;
	} else {
		shell_error(shell, "ov2640 - output format not supported, error: %d", err);
	}

	if (argc == 2 || (argc == 3 && !strcmp(argv[2], "both"))) {
		shell_print(shell, "ov2640 (both) - set output format to %s...", argv[1]);
		err |= video_set_format(ov2640_dev_1, VIDEO_EP_OUT, &fmt_1);
		err |= video_set_format(ov2640_dev_2, VIDEO_EP_OUT, &fmt_2);
	} else if (!strcmp(argv[2], "left")) {
		shell_print(shell, "ov2640 (left) - set output format to %s...", argv[1]);
		err |= video_set_format(ov2640_dev_1, VIDEO_EP_OUT, &fmt_1);
	} else if (!strcmp(argv[2], "right")) {
		shell_print(shell, "ov2640 (right) - set output format to %s...", argv[1]);
		err |= video_set_format(ov2640_dev_2, VIDEO_EP_OUT, &fmt_2);
	} else {
		shell_error(shell, "Wrong function parameters.");
	}

	if (err) {
		shell_error(shell, "ov2640 - set output format failed, error: %d", err);
	}

	shell_print(shell, "Finished.");

	return err;
}

static int cmd_ov2640_set_quality(const struct shell *shell, size_t argc, char **argv)
{
	int err = 0;
    int arg = atoi(argv[1]);

	if (argc == 2 || (argc == 3 && !strcmp(argv[2], "both"))) {
		shell_print(shell, "ov2640 (both) - set quality to %s...", argv[1]);
		err |= video_set_ctrl(ov2640_dev_1, VIDEO_CID_CAMERA_QUALITY, &arg);
		err |= video_set_ctrl(ov2640_dev_2, VIDEO_CID_CAMERA_QUALITY, &arg);
	} 
	else if (!strcmp(argv[2], "left")) {
		shell_print(shell, "ov2640 (left) - set quality to %s...", argv[1]);
		err |= video_set_ctrl(ov2640_dev_1, VIDEO_CID_CAMERA_QUALITY, &arg);
	} 
	else if (!strcmp(argv[2], "right")) {
		shell_print(shell, "ov2640 (right) - set quality to %s...", argv[1]);
		err |= video_set_ctrl(ov2640_dev_2, VIDEO_CID_CAMERA_QUALITY, &arg);
	} else {
		shell_error(shell, "Wrong function parameters.");
	}

	if (err) {
		shell_error(shell, "ov2640 - set quality failed, error: %d", err);
	}

	shell_print(shell, "Finished.");

	return err;
}

static int cmd_ov2640_set_colorbar(const struct shell *shell, size_t argc, char **argv)
{
	int err = 0;
    int arg = atoi(argv[1]);

	if (argc == 2 || (argc == 3 && !strcmp(argv[2], "both"))) {
		shell_print(shell, "ov2640 (both) - set colorbar to %s...", argv[1]);
		err |= video_set_ctrl(ov2640_dev_1, VIDEO_CID_CAMERA_COLORBAR, &arg);
		err |= video_set_ctrl(ov2640_dev_2, VIDEO_CID_CAMERA_COLORBAR, &arg);
	} else if (!strcmp(argv[2], "left")) {
		shell_print(shell, "ov2640 (left) - set colorbar to %s...", argv[1]);
		err |= video_set_ctrl(ov2640_dev_1, VIDEO_CID_CAMERA_COLORBAR, &arg);
	} else if (!strcmp(argv[2], "right")) {
		shell_print(shell, "ov2640 (right) - set colorbar to %s...", argv[1]);
		err |= video_set_ctrl(ov2640_dev_2, VIDEO_CID_CAMERA_COLORBAR, &arg);
	} else {
		shell_error(shell, "Wrong function parameters.");
	}

	if (err) {
		shell_error(shell, "ov2640 - set colorbar failed, error: %d", err);
	}

	shell_print(shell, "Finished.");

	return err;
}

static int cmd_ov2640_set_whitebal(const struct shell *shell, size_t argc, char **argv)
{
	int err = 0;
    int arg = atoi(argv[1]);

	if (argc == 2 || (argc == 3 && !strcmp(argv[2], "both"))) {
		shell_print(shell, "ov2640 (both) - set white balance to %s...", argv[1]);
		err |= video_set_ctrl(ov2640_dev_1, VIDEO_CID_CAMERA_WHITE_BAL, &arg);
		err |= video_set_ctrl(ov2640_dev_2, VIDEO_CID_CAMERA_WHITE_BAL, &arg);
	} else if (!strcmp(argv[2], "left")) {
		shell_print(shell, "ov2640 (left) - set white balance to %s...", argv[1]);
		err |= video_set_ctrl(ov2640_dev_1, VIDEO_CID_CAMERA_WHITE_BAL, &arg);
	} else if (!strcmp(argv[2], "right")) {
		shell_print(shell, "ov2640 (right) - set white balance to %s...", argv[1]);
		err |= video_set_ctrl(ov2640_dev_2, VIDEO_CID_CAMERA_WHITE_BAL, &arg);
	} else {
		shell_error(shell, "Wrong function parameters.");
	}

	if (err) {
		shell_error(shell, "ov2640 - set white balance failed, error: %d", err);
	}

	shell_print(shell, "Finished.");

	return err;
}

static int cmd_ov2640_set_gainctrl(const struct shell *shell, size_t argc, char **argv)
{
	int err = 0, ret = 0;
    int arg = atoi(argv[1]);

	if (argc == 2 || (argc == 3 && !strcmp(argv[2], "both"))) {
		shell_print(shell, "ov2640 (both) - set gain to %s...", argv[1]);
		ret |= video_set_ctrl(ov2640_dev_1, VIDEO_CID_CAMERA_GAIN, &arg);
		ret |= video_set_ctrl(ov2640_dev_2, VIDEO_CID_CAMERA_GAIN, &arg);
	} else if (!strcmp(argv[2], "left")) {
		shell_print(shell, "ov2640 (left) - set gain to %s...", argv[1]);
		ret |= video_set_ctrl(ov2640_dev_1, VIDEO_CID_CAMERA_GAIN, &arg);
	} else if (!strcmp(argv[2], "right")) {
		shell_print(shell, "ov2640 (right) - set gain to %s...", argv[1]);
		ret |= video_set_ctrl(ov2640_dev_2, VIDEO_CID_CAMERA_GAIN, &arg);
	} else {
		shell_error(shell, "Wrong function parameters.");
		err = -1;
	}

	if (err | ret) {
		shell_error(shell, "ov2640 - set gain failed, error: %d, ret: %d", err, ret);
	}

	shell_print(shell, "Finished.");

	return err | ret;
}

static int cmd_ov2640_set_exposurectrl(const struct shell *shell, size_t argc,
				char **argv)
{
	int err = 0, ret = 0;
    int arg = atoi(argv[1]);

	if (argc == 2 || (argc == 3 && !strcmp(argv[2], "both"))) {
		shell_print(shell, "ov2640 (both) - set exposure to %s...", argv[1]);
		ret |= video_set_ctrl(ov2640_dev_1, VIDEO_CID_CAMERA_EXPOSURE, &arg);
		ret |= video_set_ctrl(ov2640_dev_2, VIDEO_CID_CAMERA_EXPOSURE, &arg);
	} else if (!strcmp(argv[2], "left")) {
		shell_print(shell, "ov2640 (left) - set exposure to %s...", argv[1]);
		ret |= video_set_ctrl(ov2640_dev_1, VIDEO_CID_CAMERA_EXPOSURE, &arg);
	} else if (!strcmp(argv[2], "right")) {
		shell_print(shell, "ov2640 (right) - set exposure to %s...", argv[1]);
		ret |= video_set_ctrl(ov2640_dev_2, VIDEO_CID_CAMERA_EXPOSURE, &arg);
	} else {
		shell_error(shell, "Wrong function parameters.");
		err = -1;
	}

	if (err | ret) {
		shell_error(shell, "ov2640 - set exposure failed, error: %d, ret: %d", err, ret);
	}

	shell_print(shell, "Finished.");

	return err | ret;
}

static int cmd_ov2640_set_horizontal_mirror(const struct shell *shell, size_t argc,
				     char **argv)
{
	int err = 0, ret = 0;
    int arg = atoi(argv[1]);

	if (argc == 2 || (argc == 3 && !strcmp(argv[2], "both"))) {
		shell_print(shell, "ov2640 (both) - set horizontal mirror to %s...", argv[1]);
		ret |= video_set_ctrl(ov2640_dev_1, VIDEO_CID_HFLIP, &arg);
		ret |= video_set_ctrl(ov2640_dev_2, VIDEO_CID_HFLIP, &arg);
	} else if (!strcmp(argv[2], "left")) {
		shell_print(shell, "ov2640 (left) - set horizontal mirror to %s...", argv[1]);
		ret |= video_set_ctrl(ov2640_dev_1, VIDEO_CID_HFLIP, &arg);
	} else if (!strcmp(argv[2], "right")) {
		shell_print(shell, "ov2640 (right) - set horizontal mirror to %s...", argv[1]);
		ret |= video_set_ctrl(ov2640_dev_2, VIDEO_CID_HFLIP, &arg);
	} else {
		shell_error(shell, "Wrong function parameters.");
		err = -1;
	}

	if (err) {
		shell_error(shell, "ov2640 - set horizontal mirror failed, error: %d, ret: %d", err, ret);
	}

	shell_print(shell, "Finished.");

	return err;
}

static int cmd_ov2640_set_vertical_flip(const struct shell *shell, size_t argc,
				 char **argv)
{
	int err = 0, ret = 0;
    int arg = atoi(argv[1]);

	if (argc == 2 || (argc == 3 && !strcmp(argv[2], "both"))) {
		shell_print(shell, "ov2640 (both) - set vertical flip to %s...", argv[1]);
		ret |= video_set_ctrl(ov2640_dev_1, VIDEO_CID_VFLIP, &arg);
		ret |= video_set_ctrl(ov2640_dev_2, VIDEO_CID_VFLIP, &arg);
	} else if (!strcmp(argv[2], "left")) {
		shell_print(shell, "ov2640 (left) - set vertical flip to %s...", argv[1]);
		ret |= video_set_ctrl(ov2640_dev_1, VIDEO_CID_VFLIP, &arg);
	} else if (!strcmp(argv[2], "right")) {
		shell_print(shell, "ov2640 (right) - set vertical flip to %s...", argv[1]);
		ret |= video_set_ctrl(ov2640_dev_2, VIDEO_CID_VFLIP, &arg);
	} else {
		shell_error(shell, "Wrong function parameters.");
		err = -1;
	}

	if (err) {
		shell_error(shell, "ov2640 - set vertical flip failed, error: %d, ret: %d", err, ret);
	}

	shell_print(shell, "Finished.");

	return err | ret;
}

static int cmd_ov2640_set_resolution(const struct shell *shell, size_t argc,
			      char **argv)
{
	int err = 0, ret = 0;
	int i = 0;
    int width = atoi(argv[1]), height = atoi(argv[2]);

	if (argc == 3 || (argc == 4 && !strcmp(argv[3], "both"))) {
		shell_print(shell, "ov2640 (both) - set resolution to %sx%s...", argv[1], argv[2]);
		while (caps.format_caps[i].pixelformat) {
			const struct video_format_cap *fcap = &caps.format_caps[i];
			if (fcap->width_min == width && fcap->height_min == height) {
				fmt_1.width = width;
				fmt_1.height = height;
				fmt_2.width = width;
				fmt_2.height = height;
				img_length_1 = width * height;
				img_length_2 = width * height;
				
				/* set image height */
				dma_block_cfg_cam.source_gather_count = height;
				dma_block_cfg_cam.dest_scatter_count = height;

				/*set block size, driver will get image width from that */
				dma_block_cfg_cam.block_size = width * height;
				dma_block_cfg_cam.source_address = 0;
				dma_block_cfg_cam.dest_address = (uint32_t)&img_buff_1;
				dma_config(fastvdma_dev_cam_1, 0, &dma_cfg_cam);
				dma_block_cfg_cam.source_address = 0;
				dma_block_cfg_cam.dest_address = (uint32_t)&img_buff_2;
				dma_config(fastvdma_dev_cam_2, 0, &dma_cfg_cam);
			}
			i++;
		}
		ret |= video_set_format(ov2640_dev_1, VIDEO_EP_OUT, &fmt_1);
		ret |= video_set_format(ov2640_dev_2, VIDEO_EP_OUT, &fmt_2);
	} else if (strcmp(argv[3], "left")) {
		shell_print(shell, "ov2640 (left) - set resolution to %sx%s...", argv[1], argv[2]);
		while (caps.format_caps[i].pixelformat) {
			const struct video_format_cap *fcap = &caps.format_caps[i];
			if (fcap->width_min == width && fcap->height_min == height) {
				fmt_1.width = width;
				fmt_1.height = height;
				img_length_1 = width * height;
				
				/* set image height */
				dma_block_cfg_cam.source_gather_count = height;
				dma_block_cfg_cam.dest_scatter_count = height;

				/*set block size, driver will get image width from that */
				dma_block_cfg_cam.block_size = width * height;
				dma_block_cfg_cam.source_address = 0;
				dma_block_cfg_cam.dest_address = (uint32_t)&img_buff_1;
				dma_config(fastvdma_dev_cam_1, 0, &dma_cfg_cam);
			}
			i++;
		}
		ret |= video_set_format(ov2640_dev_1, VIDEO_EP_OUT, &fmt_1);
	} else if (strcmp(argv[3], "right")) {
		shell_print(shell, "ov2640 (right) - set resolution to %sx%s...", argv[1], argv[2]);
		while (caps.format_caps[i].pixelformat) {
			const struct video_format_cap *fcap = &caps.format_caps[i];
			if (fcap->width_min == width && fcap->height_min == height) {
				fmt_2.width = width;
				fmt_2.height = width;
				img_length_2 = width * height;
				
				/* set image height */
				dma_block_cfg_cam.source_gather_count = height;
				dma_block_cfg_cam.dest_scatter_count = height;

				/*set block size, driver will get image width from that */
				dma_block_cfg_cam.block_size = width * height;
				dma_block_cfg_cam.source_address = 0;
				dma_block_cfg_cam.dest_address = (uint32_t)&img_buff_2;
				dma_config(fastvdma_dev_cam_2, 0, &dma_cfg_cam);
			}
			i++;
		}
		ret |= video_set_format(ov2640_dev_2, VIDEO_EP_OUT, &fmt_2);
	} else {
		shell_error(shell, "Wrong function parameters.");
		err = -1;
	}

	shell_print(shell, " - (OV2640_1) Current format: %c%c%c%c %ux%u\n", (char)fmt_1.pixelformat,
	       (char)(fmt_1.pixelformat >> 8),
	       (char)(fmt_1.pixelformat >> 16),
	       (char)(fmt_1.pixelformat >> 24),
	       fmt_1.width, fmt_1.height);

	shell_print(shell, " - (OV2640_2) Current format: %c%c%c%c %ux%u\n", (char)fmt_2.pixelformat,
	       (char)(fmt_2.pixelformat >> 8),
	       (char)(fmt_2.pixelformat >> 16),
	       (char)(fmt_2.pixelformat >> 24),
	       fmt_2.width, fmt_2.height);

	if (err | ret) {
		shell_error(shell, "ov2640 - set resolution failed, error: %d, ret: %d", err, ret);
	}

	shell_print(shell, "Finished.");

	return err | ret;
}

static int cmd_ov2640_send_image(const struct shell *shell, size_t argc,
				char **argv)
{
	int err = 0;

	if (argc == 1 || (argc == 2 && !strcmp(argv[1], "both"))) {
		shell_print(shell, "ov2640 (both) - send image...");
		send_image((uint32_t*)&img_buff_1, img_length_1);
		send_image((uint32_t*)&img_buff_2, img_length_2);
	} else if (!strcmp(argv[1], "left")) {
		shell_print(shell, "ov2640 (left) - send image...");
		send_image((uint32_t*)&img_buff_1, img_length_1);
	} else if (!strcmp(argv[1], "right")) {
		shell_print(shell, "ov2640 (right) - send image...");
		send_image((uint32_t*)&img_buff_2, img_length_2);
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

static int cmd_ov2640_capture(const struct shell *shell, size_t argc,
				char **argv)
{
	ARG_UNUSED(argv);
	int err = 0, ret = 0;

	flush_l2_cache();
	if (argc == 1 || (argc == 2 && !strcmp(argv[1], "both"))) {
		shell_print(shell, "ov2640 (both) - capture image...");
		ret |= dma_start(fastvdma_dev_cam_1, 0);
		ret |= dma_start(fastvdma_dev_cam_2, 0);
	} else if (!strcmp(argv[1], "left")) {
		shell_print(shell, "ov2640 (left) - capture image...");
		ret |= dma_start(fastvdma_dev_cam_1, 0);
	} else if (!strcmp(argv[1], "right")) {
		shell_print(shell, "ov2640 (right) - capture image...");
		ret |= dma_start(fastvdma_dev_cam_2, 0);
	} else {
		shell_error(shell, "Wrong function parameters.");
		err = -1;
	}

	if (err | ret) {
		shell_error(shell, "ov2640 - capture image failed, error: %d, ret: %d", err, ret);
	} else {
		shell_print(shell, "Capture initialized.");
	}

	return err | ret;
}

SHELL_STATIC_SUBCMD_SET_CREATE(
	sub_ov2640_control,
	SHELL_CMD_ARG(brightness, NULL, "\t(-2 to +2) Set brightness level", cmd_ov2640_set_brightness, 2, 1),
	SHELL_CMD_ARG(saturation, NULL, "\t(-2 to +2) Set saturation level", cmd_ov2640_set_saturation, 2, 1),
	SHELL_CMD_ARG(output_format, NULL, "\t[JPEG|RGB565] Set output format", cmd_ov2640_set_output_format, 2, 1),
	SHELL_CMD_ARG(quality, NULL, "\t(0 to 255) Set quality level", cmd_ov2640_set_quality, 2, 1),
	SHELL_CMD_ARG(colorbar, NULL, "\t[1|0] Enable/disable test colorbar", cmd_ov2640_set_colorbar, 2, 1),
	SHELL_CMD_ARG(whitebal, NULL, "\t[1|0] Enable/disable automatic white balance", cmd_ov2640_set_whitebal, 2, 1),
	SHELL_CMD_ARG(gainctrl, NULL, "\t[1|0] Enable/disable automatic gain control", cmd_ov2640_set_gainctrl, 2, 1),
	SHELL_CMD_ARG(exposurectrl, NULL, "\t[1|0] Enable/disable automatic exposure control", cmd_ov2640_set_exposurectrl,
		      2, 1),
	SHELL_CMD_ARG(hflip, NULL, "\t[1|0] Enable/disable horizontal flip", cmd_ov2640_set_horizontal_mirror, 2, 1),
	SHELL_CMD_ARG(vflip, NULL, "\t[1|0] Enable/disable vertical flip", cmd_ov2640_set_vertical_flip, 2, 1),
	SHELL_CMD_ARG(resolution, NULL, "\t[width height] Set resolution", cmd_ov2640_set_resolution, 3, 1),
	SHELL_SUBCMD_SET_END);

SHELL_STATIC_SUBCMD_SET_CREATE(
	sub_ov2640, 
	SHELL_CMD(control, &sub_ov2640_control, "\tov2640 control commands", NULL),
	SHELL_CMD_ARG(send_image, NULL, "\tSend image via UPD at 192.0.2.2. "
        "Provide either left or right to choose which buffer should be sent. "
        "If none provided these will be sent one after another starting with the left one.\n",
        cmd_ov2640_send_image, 1, 1),
	SHELL_CMD_ARG(capture, NULL, "\tInitialize camera capture. "
        "Provide either left or right to choose which camera image should be initialized.\n",
        cmd_ov2640_capture, 1, 1),
	SHELL_SUBCMD_SET_END);

SHELL_CMD_REGISTER(ov2640, &sub_ov2640, "\tov2640 commands", NULL);
