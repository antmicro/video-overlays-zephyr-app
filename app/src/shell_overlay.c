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
extern enum overlay_mode {text, logo} overlay_mode;
extern enum overlay_mode overlay_mode;

static int cmd_set_text(const struct shell *shell, size_t argc, char **argv)
{
	strncpy(overlay_text, argv[1], 25);
	shell_print(shell, "Overlay text set to '%s'", overlay_text);

	return 0;
}

static int cmd_set_mode(const struct shell *shell, size_t argc, char **argv)
{
	if (!strcmp(argv[1], "text")) {
		shell_print(shell, "Setting mode to text");
		overlay_mode = text;
	} else if (!strcmp(argv[1], "logo")) {
		shell_print(shell, "Setting mode to logo");
		overlay_mode = logo;
	} else {
		shell_error(shell, "Wrong function parameters.");
	}
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
	SHELL_CMD_ARG(set_mode, NULL, "\tSet overlay mode - text/logo", cmd_set_mode, 2, 0),
	SHELL_CMD_ARG(print_text, NULL, "\tPrint overlay text", cmd_print_text, 1, 0),
	SHELL_SUBCMD_SET_END);

SHELL_CMD_REGISTER(overlay, &sub_overlay, "\tConfigure overlay.", NULL);
