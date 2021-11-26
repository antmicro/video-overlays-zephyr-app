#include <zephyr.h>
#include <device.h>
#include <shell/shell.h>
#include <stdlib.h>
#include <net/socket.h>
#include <drivers/video.h>
#include <drivers/dma.h>
#include <string.h>

#include "pattern.h"
#include "initiator.h"
#include "mmcm.h"
#include "time.h"

#include "net.h"

extern const struct device* ov2640_dev_1;
extern const struct device* ov2640_dev_2;
extern const struct device* fastvdma_dev_1;
extern const struct device* fastvdma_dev_2;
extern uint32_t img_buff_1[800 * 600];
extern uint32_t img_buff_2[800 * 600];
extern uint32_t img_buff_3[800 * 600];
extern uint32_t img_length_1;
extern uint32_t img_length_2;
extern struct video_format fmt_1;
extern struct video_format fmt_2;
extern struct video_caps caps;
extern struct dma_config dma_cfg;
extern struct dma_block_config dma_block_cfg;

#define CONFIG_L2_SIZE 8192
#define MAIN_RAM_BASE 0x40000000L
#define BUFFER_ADDR 0x200000
#define GEN_PURPOSE_1   		(*(volatile uint32_t *)(0xf000388c))
struct video_timing vt1920x1080_60Hz = {
	.pixel_clock = 14850,

	.h_active = 1920,
	.h_blanking = 280,
	.h_sync_offset = 88,
	.h_sync_width = 44,

	.v_active = 1080,
	.v_blanking = 45,
	.v_sync_offset = 4,
	.v_sync_width = 5,
};
struct video_timing vt1280x720_60Hz = {

	.pixel_clock = 7425,

	.h_active = 1280,
	.h_blanking = 370,
	.h_sync_offset = 110,
	.h_sync_width = 40,

	.v_active = 720,
	.v_blanking = 30,
	.v_sync_offset = 5,
	.v_sync_width = 5,
};
struct video_timing vt800x600_60Hz = {

	.pixel_clock = 4000,

	.h_active = 800,
	.h_blanking = 256,
	.h_sync_offset = 40,
	.h_sync_width = 128,

	.v_active = 600,
	.v_blanking = 28,
	.v_sync_offset = 1,
	.v_sync_width = 4,
};
struct video_timing vt640x480_75Hz = {

	.pixel_clock = 3150,

	.h_active = 640,     
	.h_blanking = 200, 
	.h_sync_offset = 16, 
	.h_sync_width = 64,

	.v_active = 480,     
	.v_blanking = 20,	
	.v_sync_offset = 1,  
	.v_sync_width = 3,
};
struct video_timing vt640x480_60Hz = {

	.pixel_clock = 2517,

	.h_active = 640,     
	.h_blanking = 160, 
	.h_sync_offset = 16, 
	.h_sync_width = 96,

	.v_active = 480,     
	.v_blanking = 45,	
	.v_sync_offset = 10,  
	.v_sync_width = 2,

};


void flush_l2_cache(void)
{
#ifdef CONFIG_L2_SIZE
	unsigned int i;
	for(i=0;i<2*CONFIG_L2_SIZE/4;i++) {
		((volatile unsigned int *) MAIN_RAM_BASE)[i];
	}
#endif
}

static int cmd_ov2640_set_brightness(const struct shell *shell, size_t argc,
			      char **argv)
{
	int err = 0;

	if (argc == 2 || (argc == 3 && !strcmp(argv[2], "both"))) {
		shell_print(shell, "ov2640 (both) - set brightness to %s...", argv[1]);
		err |= video_set_ctrl(ov2640_dev_1,
			VIDEO_CID_CAMERA_BRIGHTNESS, atoi(argv[1]));
		err |= video_set_ctrl(ov2640_dev_2,
			VIDEO_CID_CAMERA_BRIGHTNESS, atoi(argv[1]));	
	} else if (!strcmp(argv[2], "left")) {
		shell_print(shell, "ov2640 (left) - set brightness to %s...", argv[1]);
		err |= video_set_ctrl(ov2640_dev_1,
			VIDEO_CID_CAMERA_BRIGHTNESS, atoi(argv[1]));
	} else if (!strcmp(argv[2], "right")) {
		shell_print(shell, "ov2640 (right) - set brightness to %s...", argv[1]);
		err |= video_set_ctrl(ov2640_dev_2,
			VIDEO_CID_CAMERA_BRIGHTNESS, atoi(argv[1]));
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

	if (argc == 2 || (argc == 3 && !strcmp(argv[2], "both"))) {
		shell_print(shell, "ov2640 (both) - set saturation to %s...", argv[1]);
		err |= video_set_ctrl(ov2640_dev_1,
			VIDEO_CID_CAMERA_SATURATION, atoi(argv[1]));
		err |= video_set_ctrl(ov2640_dev_2,
			VIDEO_CID_CAMERA_SATURATION, atoi(argv[1]));	
	} else if (!strcmp(argv[2], "left")) {
		shell_print(shell, "ov2640 (left) - set saturation to %s...", argv[1]);
		err |= video_set_ctrl(ov2640_dev_1,
			VIDEO_CID_CAMERA_SATURATION, atoi(argv[1]));
	} else if (!strcmp(argv[2], "right")) {
		shell_print(shell, "ov2640 (right) - set saturation to %s...", argv[1]);
		err |= video_set_ctrl(ov2640_dev_2,
			VIDEO_CID_CAMERA_SATURATION, atoi(argv[1]));
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

	if (argv[1] == "JPEG") {
		fmt_1.pixelformat = VIDEO_PIX_FMT_JPEG;
		fmt_2.pixelformat = VIDEO_PIX_FMT_JPEG;
	} else if (argv[1] == "RGB565") {
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

	if (argc == 2 || (argc == 3 && !strcmp(argv[2], "both"))) {
		shell_print(shell, "ov2640 (both) - set quality to %s...", argv[1]);
		err |= video_set_ctrl(ov2640_dev_1, VIDEO_CID_CAMERA_QUALITY, atoi(argv[1]));
		err |= video_set_ctrl(ov2640_dev_2, VIDEO_CID_CAMERA_QUALITY, atoi(argv[1]));
	} 
	else if (!strcmp(argv[2], "left")) {
		shell_print(shell, "ov2640 (left) - set quality to %s...", argv[1]);
		err |= video_set_ctrl(ov2640_dev_1, VIDEO_CID_CAMERA_QUALITY, atoi(argv[1]));
	} 
	else if (!strcmp(argv[2], "right")) {
		shell_print(shell, "ov2640 (right) - set quality to %s...", argv[1]);
		err |= video_set_ctrl(ov2640_dev_2, VIDEO_CID_CAMERA_QUALITY, atoi(argv[1]));
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

	if (argc == 2 || (argc == 3 && !strcmp(argv[2], "both"))) {
		shell_print(shell, "ov2640 (both) - set colorbar to %s...", argv[1]);
		err |= video_set_ctrl(ov2640_dev_1, VIDEO_CID_CAMERA_COLORBAR, atoi(argv[1]));
		err |= video_set_ctrl(ov2640_dev_2, VIDEO_CID_CAMERA_COLORBAR, atoi(argv[1]));
	} else if (!strcmp(argv[2], "left")) {
		shell_print(shell, "ov2640 (left) - set colorbar to %s...", argv[1]);
		err |= video_set_ctrl(ov2640_dev_1, VIDEO_CID_CAMERA_COLORBAR, atoi(argv[1]));
	} else if (!strcmp(argv[2], "right")) {
		shell_print(shell, "ov2640 (right) - set colorbar to %s...", argv[1]);
		err |= video_set_ctrl(ov2640_dev_2, VIDEO_CID_CAMERA_COLORBAR, atoi(argv[1]));
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

	if (argc == 2 || (argc == 3 && !strcmp(argv[2], "both"))) {
		shell_print(shell, "ov2640 (both) - set white balance to %s...", argv[1]);
		err |= video_set_ctrl(ov2640_dev_1, VIDEO_CID_CAMERA_WHITE_BAL, atoi(argv[1]));
		err |= video_set_ctrl(ov2640_dev_2, VIDEO_CID_CAMERA_WHITE_BAL, atoi(argv[1]));
	} else if (!strcmp(argv[2], "left")) {
		shell_print(shell, "ov2640 (left) - set white balance to %s...", argv[1]);
		err |= video_set_ctrl(ov2640_dev_1, VIDEO_CID_CAMERA_WHITE_BAL, atoi(argv[1]));
	} else if (!strcmp(argv[2], "right")) {
		shell_print(shell, "ov2640 (right) - set white balance to %s...", argv[1]);
		err |= video_set_ctrl(ov2640_dev_2, VIDEO_CID_CAMERA_WHITE_BAL, atoi(argv[1]));
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

	if (argc == 2 || (argc == 3 && !strcmp(argv[2], "both"))) {
		shell_print(shell, "ov2640 (both) - set gain to %s...", argv[1]);
		ret |= video_set_ctrl(ov2640_dev_1, VIDEO_CID_CAMERA_GAIN, atoi(argv[1]));
		ret |= video_set_ctrl(ov2640_dev_2, VIDEO_CID_CAMERA_GAIN, atoi(argv[1]));
	} else if (!strcmp(argv[2], "left")) {
		shell_print(shell, "ov2640 (left) - set gain to %s...", argv[1]);
		ret |= video_set_ctrl(ov2640_dev_1, VIDEO_CID_CAMERA_GAIN, atoi(argv[1]));
	} else if (!strcmp(argv[2], "right")) {
		shell_print(shell, "ov2640 (right) - set gain to %s...", argv[1]);
		ret |= video_set_ctrl(ov2640_dev_2, VIDEO_CID_CAMERA_GAIN, atoi(argv[1]));
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

	if (argc == 2 || (argc == 3 && !strcmp(argv[2], "both"))) {
		shell_print(shell, "ov2640 (both) - set exposure to %s...", argv[1]);
		ret |= video_set_ctrl(ov2640_dev_1, VIDEO_CID_CAMERA_EXPOSURE, atoi(argv[1]));
		ret |= video_set_ctrl(ov2640_dev_2, VIDEO_CID_CAMERA_EXPOSURE, atoi(argv[1]));
	} else if (!strcmp(argv[2], "left")) {
		shell_print(shell, "ov2640 (left) - set exposure to %s...", argv[1]);
		ret |= video_set_ctrl(ov2640_dev_1, VIDEO_CID_CAMERA_EXPOSURE, atoi(argv[1]));
	} else if (!strcmp(argv[2], "right")) {
		shell_print(shell, "ov2640 (right) - set exposure to %s...", argv[1]);
		ret |= video_set_ctrl(ov2640_dev_2, VIDEO_CID_CAMERA_EXPOSURE, atoi(argv[1]));
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

	if (argc == 2 || (argc == 3 && !strcmp(argv[2], "both"))) {
		shell_print(shell, "ov2640 (both) - set horizontal mirror to %s...", argv[1]);
		ret |= video_set_ctrl(ov2640_dev_1, VIDEO_CID_HFLIP, atoi(argv[1]));
		ret |= video_set_ctrl(ov2640_dev_2, VIDEO_CID_HFLIP, atoi(argv[1]));
	} else if (!strcmp(argv[2], "left")) {
		shell_print(shell, "ov2640 (left) - set horizontal mirror to %s...", argv[1]);
		ret |= video_set_ctrl(ov2640_dev_1, VIDEO_CID_HFLIP, atoi(argv[1]));
	} else if (!strcmp(argv[2], "right")) {
		shell_print(shell, "ov2640 (right) - set horizontal mirror to %s...", argv[1]);
		ret |= video_set_ctrl(ov2640_dev_2, VIDEO_CID_HFLIP, atoi(argv[1]));
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

	if (argc == 2 || (argc == 3 && !strcmp(argv[2], "both"))) {
		shell_print(shell, "ov2640 (both) - set vertical flip to %s...", argv[1]);
		ret |= video_set_ctrl(ov2640_dev_1, VIDEO_CID_VFLIP, atoi(argv[1]));
		ret |= video_set_ctrl(ov2640_dev_2, VIDEO_CID_VFLIP, atoi(argv[1]));
	} else if (!strcmp(argv[2], "left")) {
		shell_print(shell, "ov2640 (left) - set vertical flip to %s...", argv[1]);
		ret |= video_set_ctrl(ov2640_dev_1, VIDEO_CID_VFLIP, atoi(argv[1]));
	} else if (!strcmp(argv[2], "right")) {
		shell_print(shell, "ov2640 (right) - set vertical flip to %s...", argv[1]);
		ret |= video_set_ctrl(ov2640_dev_2, VIDEO_CID_VFLIP, atoi(argv[1]));
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

	if (argc == 3 || (argc == 4 && !strcmp(argv[3], "both"))) {
		shell_print(shell, "ov2640 (both) - set resolution to %sx%s...", argv[1], argv[2]);
		while (caps.format_caps[i].pixelformat) {
			const struct video_format_cap *fcap = &caps.format_caps[i];
			if (fcap->width_min == atoi(argv[1]) && fcap->height_min == atoi(argv[2])) {
				fmt_1.width = atoi(argv[1]);
				fmt_1.height = atoi(argv[2]);
				fmt_2.width = atoi(argv[1]);
				fmt_2.height = atoi(argv[2]);
				img_length_1 = atoi(argv[1]) * atoi(argv[2]);
				img_length_2 = atoi(argv[1]) * atoi(argv[2]);
				
				/* set image height */
				dma_block_cfg.source_gather_count = atoi(argv[2]);
				dma_block_cfg.dest_scatter_count = atoi(argv[2]);

				/*set block size, driver will get image width from that */
				dma_block_cfg.block_size = atoi(argv[1]) * atoi(argv[2]);
				dma_block_cfg.dest_address = &img_buff_1;
				dma_config(fastvdma_dev_1, 0, &dma_cfg);
				dma_block_cfg.dest_address = &img_buff_2;
				dma_config(fastvdma_dev_2, 0, &dma_cfg);
			}
			i++;
		}
		ret |= video_set_format(ov2640_dev_1, VIDEO_EP_OUT, &fmt_1);
		ret |= video_set_format(ov2640_dev_2, VIDEO_EP_OUT, &fmt_2);
	} else if (strcmp(argv[3], "left")) {
		shell_print(shell, "ov2640 (left) - set resolution to %sx%s...", argv[1], argv[2]);
		while (caps.format_caps[i].pixelformat) {
			const struct video_format_cap *fcap = &caps.format_caps[i];
			if (fcap->width_min == atoi(argv[1]) && fcap->height_min == atoi(argv[2])) {
				fmt_1.width = atoi(argv[1]);
				fmt_1.height = atoi(argv[2]);
				img_length_1 = atoi(argv[1]) * atoi(argv[2]);
				
				/* set image height */
				dma_block_cfg.source_gather_count = atoi(argv[2]);
				dma_block_cfg.dest_scatter_count = atoi(argv[2]);

				/*set block size, driver will get image width from that */
				dma_block_cfg.block_size = atoi(argv[1]) * atoi(argv[2]);
				dma_block_cfg.dest_address = &img_buff_1;
				dma_config(fastvdma_dev_1, 0, &dma_cfg);
			}
			i++;
		}
		ret |= video_set_format(ov2640_dev_1, VIDEO_EP_OUT, &fmt_1);
	} else if (strcmp(argv[3], "right")) {
		shell_print(shell, "ov2640 (right) - set resolution to %sx%s...", argv[1], argv[2]);
		while (caps.format_caps[i].pixelformat) {
			const struct video_format_cap *fcap = &caps.format_caps[i];
			if (fcap->width_min == atoi(argv[1]) && fcap->height_min == atoi(argv[2])) {
				fmt_2.width = atoi(argv[1]);
				fmt_2.height = atoi(argv[2]);
				img_length_2 = atoi(argv[1]) * atoi(argv[2]);
				
				/* set image height */
				dma_block_cfg.source_gather_count = atoi(argv[2]);
				dma_block_cfg.dest_scatter_count = atoi(argv[2]);

				/*set block size, driver will get image width from that */
				dma_block_cfg.block_size = atoi(argv[1]) * atoi(argv[2]);
				dma_block_cfg.dest_address = &img_buff_2;
				dma_config(fastvdma_dev_2, 0, &dma_cfg);
			}
			i++;
		}
		ret |= video_set_format(ov2640_dev_2, VIDEO_EP_OUT, &fmt_2);
	} else {
		shell_error(shell, "Wrong function parameters.");
		err = -1;
	}

	printf(" - (OV2640_1) Current format: %c%c%c%c %ux%u\n", (char)fmt_1.pixelformat,
	       (char)(fmt_1.pixelformat >> 8),
	       (char)(fmt_1.pixelformat >> 16),
	       (char)(fmt_1.pixelformat >> 24),
	       fmt_1.width, fmt_1.height);

	printf(" - (OV2640_2) Current format: %c%c%c%c %ux%u\n", (char)fmt_2.pixelformat,
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
		send_image(&img_buff_1, img_length_1);
		send_image(&img_buff_2, img_length_2);
	} else if (!strcmp(argv[1], "left")) {
		shell_print(shell, "ov2640 (left) - send image...");
		send_image(&img_buff_1, img_length_1);
	} else if (!strcmp(argv[1], "right")) {
		shell_print(shell, "ov2640 (right) - send image...");
		send_image(&img_buff_2, img_length_2);
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
		ret |= dma_start(fastvdma_dev_1, 0);
		ret |= dma_start(fastvdma_dev_2, 0);
	} else if (!strcmp(argv[1], "left")) {
		shell_print(shell, "ov2640 (left) - capture image...");
		ret |= dma_start(fastvdma_dev_1, 0);
	} else if (!strcmp(argv[1], "right")) {
		shell_print(shell, "ov2640 (right) - capture image...");
		ret |= dma_start(fastvdma_dev_2, 0);
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


int set_resolution(const struct shell *shell, size_t argc, char **argv)
{
	hdmi_out0_core_initiator_enable_write(0);
	hdmi_out0_driver_clocking_mmcm_reset_write(1);
	if(strcmp(argv[1], "1920x1080_60Hz") == 0){
		timings_write(&vt1920x1080_60Hz);	
	}
	else if (strcmp(argv[1], "1280x720_60Hz") == 0){
		timings_write(&vt1280x720_60Hz);	
	}
	else if (strcmp(argv[1], "800x600_60Hz") == 0){
		timings_write(&vt800x600_60Hz);	
	}
	else if (strcmp(argv[1], "640x480_75Hz") == 0){
		timings_write(&vt640x480_75Hz);	
	}
	else if (strcmp(argv[1], "640x480_60Hz") == 0){
		timings_write(&vt640x480_60Hz);	
	}
	else {
			shell_print(shell, "\nUnsupported resolution!\n");
			return 0;
	}
	hdmi_out0_driver_clocking_mmcm_reset_write(0);

	shell_print(shell, "\nResoluton set!\n");
    return 0;
}

int test_display_colors(const struct shell *shell, size_t argc, char **argv)
{
	hdmi_out0_core_initiator_enable_write(1);
	draw_color(640, 480, RGB_RED);
	k_msleep(1000);
	hdmi_out0_core_initiator_base_write(BUFFER_ADDR);
	k_msleep(1000);
	draw_color(640, 480, RGB_GREEN);
	k_msleep(1000);
	hdmi_out0_core_initiator_base_write(BUFFER_ADDR);
	k_msleep(1000);
	draw_color(640, 480, RGB_BLUE);
	k_msleep(1000);
	hdmi_out0_core_initiator_base_write(BUFFER_ADDR);
	k_msleep(1000);
	draw_color(640, 480, RGB_TEST);
	k_msleep(1000);
	hdmi_out0_core_initiator_base_write(BUFFER_ADDR);
	k_msleep(1000);
	hdmi_out0_core_initiator_base_write(BUFFER_ADDR);
    return 0;
}

int test_cam1(const struct shell *shell, size_t argc, char **argv)
{
	hdmi_out0_core_initiator_enable_write(1);
	shell_print(shell, "Displaying image from camera 1");
	hdmi_out0_core_initiator_base_write(&img_buff_1);
}

int test_cam2(const struct shell *shell, size_t argc, char **argv)
{
	hdmi_out0_core_initiator_enable_write(1);
	shell_print(shell, "Displaying image from camera 2");
	hdmi_out0_core_initiator_base_write(&img_buff_2);
}

int test_video(const struct shell *shell, size_t argc, char **argv)
{
	hdmi_out0_core_initiator_enable_write(1);

	flush_l2_cache();
	dma_block_cfg.dest_address = &img_buff_2;
	dma_config(fastvdma_dev_1, 0, &dma_cfg);
	dma_start(fastvdma_dev_1, 0);
	struct dma_status stat;

	while(1){
		flush_l2_cache();
		dma_block_cfg.dest_address = &img_buff_1;
		dma_config(fastvdma_dev_1, 0, &dma_cfg);
		dma_start(fastvdma_dev_1, 0);
		k_msleep(10);
		dma_get_status(fastvdma_dev_1, 0, &stat);
		while(stat.busy != 0){
			dma_get_status(fastvdma_dev_1, 0, &stat);
		}
		hdmi_out0_core_initiator_base_write(&img_buff_2);
		k_msleep(10);
		flush_l2_cache();
		dma_block_cfg.dest_address = &img_buff_3;
		dma_config(fastvdma_dev_1, 0, &dma_cfg);
		dma_start(fastvdma_dev_1, 0);
		k_msleep(10);
		dma_get_status(fastvdma_dev_1, 0, &stat);
		while(stat.busy != 0){
			dma_get_status(fastvdma_dev_1, 0, &stat);
		}
		hdmi_out0_core_initiator_base_write(&img_buff_1);
		k_msleep(10);
		flush_l2_cache();
		dma_block_cfg.dest_address = &img_buff_2;
		dma_config(fastvdma_dev_1, 0, &dma_cfg);
		dma_start(fastvdma_dev_1, 0);
		k_msleep(10);
	    dma_get_status(fastvdma_dev_1, 0, &stat);
		while(stat.busy != 0){
			dma_get_status(fastvdma_dev_1, 0, &stat);
		}
		hdmi_out0_core_initiator_base_write(&img_buff_3);
		k_msleep(10);
	}
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
	// SHELL_CMD_ARG(reset, NULL, NULL, cmd_ov2640_reset, 1, 0),
	SHELL_CMD(control, &sub_ov2640_control, "\tov2640 control commands", NULL),
	SHELL_CMD_ARG(send_image, NULL, "\tsend image via ethernet", cmd_ov2640_send_image, 1, 1),
	SHELL_CMD_ARG(capture, NULL, "\tperform capture", cmd_ov2640_capture, 1, 1),
	SHELL_SUBCMD_SET_END);

SHELL_CMD_REGISTER(ov2640, &sub_ov2640, "\tov2640 commands", NULL);

SHELL_STATIC_SUBCMD_SET_CREATE(
	sub_display, SHELL_CMD_ARG(test_display_colors, NULL, NULL, test_display_colors, 1, 0),
	SHELL_CMD_ARG(test_cam1, NULL, NULL, test_cam1, 1, 0),
	SHELL_CMD_ARG(test_cam2, NULL, NULL, test_cam2, 1, 0),
	SHELL_CMD_ARG(test_video, NULL, NULL, test_video, 1, 0),
	SHELL_CMD_ARG(set_resolution, NULL,
					    "Resolutions:\n"
					    "1920x1080_60Hz\t"
					    "1280x720_60Hz\t"
					    "800x600_60Hz\t"
					    "640x480_75Hz\n", set_resolution, 2, 1),
	SHELL_SUBCMD_SET_END);

SHELL_CMD_REGISTER(display, &sub_display, "\tdisplay commands", NULL);