/*
 * Copyright (c) 2021-2022 Antmicro
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <shell/shell.h>
#include "img_gen.h"

extern uint32_t image_with_text[1280 * 1024];
extern char overlay_text[25];
extern struct video_format fmt_1;
extern struct video_format fmt_2;

static int cmd_set_text(const struct shell *shell, size_t argc, char **argv)
{
	strncpy(overlay_text, argv[1], 25);
	shell_print(shell, "Overlay text set to '%s'", overlay_text);

	return 0;
}

static int cmd_print_text(const struct shell *shell, size_t argc, char **argv)
{
	shell_print(shell, "Overlay text: '%s'", overlay_text);

	return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(
	sub_overlay,
	SHELL_CMD_ARG(set_text, NULL, "\tSet overlay text (max 25 characters)", cmd_set_text, 2, 0),
	SHELL_CMD_ARG(print_text, NULL, "\tPrint overlay text", cmd_print_text, 1, 0),
	SHELL_SUBCMD_SET_END);

SHELL_CMD_REGISTER(overlay, &sub_overlay, "\tConfigure overlay.", NULL);
