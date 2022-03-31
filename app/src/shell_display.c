/*
 * Copyright (c) 2021-2022 Antmicro
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <shell/shell.h>
#include <drivers/video.h>
#include <stdlib.h>

#include "hdmi.h"
#include "pattern.h"

/* HDMI predefined timings */
struct video_timing vt1280x1024_60Hz = {

	.pixel_clock = 10800,

	.h_active = 1280,
	.h_blanking = 408,
	.h_sync_offset = 48,
	.h_sync_width = 112,

	.v_active = 1024,
	.v_blanking = 42,
	.v_sync_offset = 1,
	.v_sync_width = 3,
};

struct video_timing vt1024x768_60Hz = {

	.pixel_clock = 6500,

	.h_active = 1024,
	.h_blanking = 320,
	.h_sync_offset = 24,
	.h_sync_width = 136,

	.v_active = 768,
	.v_blanking = 38,
	.v_sync_offset = 3,
	.v_sync_width = 6,
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

void hdmi_reset_sequence(void)
{
	hdmi_out0_reset_regs();
	k_msleep(10);
	hdmi_out0_core_initiator_enable_write(1);
	k_msleep(10);
	hdmi_out0_core_initiator_enable_write(0);
	k_msleep(10);
}

static int cmd_display_set_resolution_1280x1024_60Hz(const struct shell *shell, size_t argc, char **argv)
{
	hdmi_reset_sequence();

	hdmi_out0_core_initiator_enable_write(0);
	hdmi_out0_driver_clocking_mmcm_reset_write(1);

	timings_write(&vt1280x1024_60Hz);

	hdmi_out0_driver_clocking_mmcm_reset_write(0);

	draw_color(gpu_buff_1, RGB_BLACK);
	k_msleep(10);
	hdmi_out0_core_initiator_base_write((uint32_t)&gpu_buff_1);
	hdmi_out0_core_initiator_enable_write(1);

	shell_print(shell, "\nResoluton 1280x1024 @ 60Hz frequency set!\n");

	return 0;
}

static int cmd_display_set_resolution_1024x768_60Hz(const struct shell *shell, size_t argc, char **argv)
{
	hdmi_reset_sequence();

	hdmi_out0_core_initiator_enable_write(0);
	hdmi_out0_driver_clocking_mmcm_reset_write(1);

	timings_write(&vt1024x768_60Hz);

	hdmi_out0_driver_clocking_mmcm_reset_write(0);

	draw_color(gpu_buff_1, RGB_BLACK);
	k_msleep(10);
	hdmi_out0_core_initiator_base_write((uint32_t)&gpu_buff_1);
	hdmi_out0_core_initiator_enable_write(1);

	shell_print(shell, "\nResoluton 1024x768 @ 60Hz frequency set!\n");

	return 0;
}

static int cmd_display_set_resolution_800x600_60Hz(const struct shell *shell, size_t argc, char **argv)
{
	hdmi_reset_sequence();

	hdmi_out0_core_initiator_enable_write(0);
	hdmi_out0_driver_clocking_mmcm_reset_write(1);

	timings_write(&vt800x600_60Hz);

	hdmi_out0_driver_clocking_mmcm_reset_write(0);

	draw_color(gpu_buff_1, RGB_BLACK);
	k_msleep(10);
	hdmi_out0_core_initiator_base_write((uint32_t)&gpu_buff_1);
	hdmi_out0_core_initiator_enable_write(1);

	shell_print(shell, "\nResoluton 800x600 @ 60Hz frequency set!\n");

	return 0;
}

static int cmd_display_set_resolution_640x480_75Hz(const struct shell *shell, size_t argc, char **argv)
{
	hdmi_reset_sequence();

	hdmi_out0_core_initiator_enable_write(0);
	hdmi_out0_driver_clocking_mmcm_reset_write(1);

	timings_write(&vt640x480_75Hz);

	hdmi_out0_driver_clocking_mmcm_reset_write(0);

	draw_color(gpu_buff_1, RGB_BLACK);
	k_msleep(10);
	hdmi_out0_core_initiator_base_write((uint32_t)&gpu_buff_1);
	hdmi_out0_core_initiator_enable_write(1);

	shell_print(shell, "\nResoluton 640x480 @ 75Hz frequency set!\n");

	return 0;
}

static int cmd_hdmi_reset(const struct shell *shell, size_t argc, char **argv)
{
	hdmi_out0_reset_write(atoi(argv[1]));

	shell_print(shell, "HDMI reset signal set to %s\n", argv[1]);

	return 0;
}

static int cmd_hdmi_reset_regs(const struct shell *shell, size_t argc, char **argv)
{
	hdmi_out0_reset_regs();

	shell_print(shell, "HDMI registers reset.\n");

	return 0;
}

static int cmd_hdmi_status(const struct shell *shell, size_t argc, char **argv)
{
	shell_print(shell, "HDMI registers:\n");

	hdmi_out0_status();

	return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(
	sub_display_resolution,
	SHELL_CMD_ARG(mode_1280x1024_60Hz, NULL, "\t1280x1024_60Hz", cmd_display_set_resolution_1280x1024_60Hz,  1, 0),
	SHELL_CMD_ARG(mode_1024x768_60Hz,  NULL, "\t1024x768_60Hz",  cmd_display_set_resolution_1024x768_60Hz,  1, 0),
	SHELL_CMD_ARG(mode_800x600_60Hz,   NULL, "\t800x600_60Hz",   cmd_display_set_resolution_800x600_60Hz,   1, 0),
	SHELL_CMD_ARG(mode_640x480_75Hz,   NULL, "\t640x480_75Hz",   cmd_display_set_resolution_640x480_75Hz,   1, 0),
	SHELL_SUBCMD_SET_END);

SHELL_STATIC_SUBCMD_SET_CREATE(
	sub_display,
	SHELL_CMD(set_resolution, &sub_display_resolution,
						"Resolutions:\n"
						"1280x1024_60Hz\t"
						"1024x768_60Hz\t"
						"800x600_60Hz\t"
						"640x480_75Hz\n", NULL),
	SHELL_CMD_ARG(reset, NULL, "\tReset signal for HDMI core. After setting this to 1, you must manually set it back to 0.\n",
	cmd_hdmi_reset, 2, 0),
	SHELL_CMD_ARG(reset_regs, NULL, "\tReset all HDMI registers to 0.\n",
	cmd_hdmi_reset_regs, 0, 0),
	SHELL_CMD_ARG(status, NULL, "\tPrint values of HDMI registers.\n",
	cmd_hdmi_status, 1, 0),
	SHELL_SUBCMD_SET_END);

SHELL_CMD_REGISTER(display, &sub_display, "\tConfigure display", NULL);
