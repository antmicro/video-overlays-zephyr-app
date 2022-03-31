/*
 * Copyright (c) 2021-2022 Antmicro
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "pattern.h"

static const unsigned int color_bar[8] = {
	RGB_WHITE,
	RGB_YELLOW,
	RGB_CYAN,
	RGB_GREEN,
	RGB_PURPLE,
	RGB_RED,
	RGB_BLUE,
	RGB_BLACK,
};

static int inc_color(int color)
{
	color++;
	return color % 8;
}

void draw_pattern(uint32_t img_buff[])
{;
	int i;
	int color;

	color = -1;
	/* color bar pattern */
	for (i = 0; i < MAX_IMG_SIZE; i++) {
		if (i % (MAX_IMG_HEIGTH / 32) == 0)
			color = inc_color(color);
		if (color >= 0)
			img_buff[i] = color_bar[color];
	}
}

void draw_color(uint32_t img_buff[], int color)
{
	/* color bar pattern */
	for (int i = 0; i < MAX_IMG_SIZE; i++) {
		img_buff[i] = color;
	}
}

void draw_colorbar(uint32_t img_buff[])
{
	/* color bar pattern */
	int temp = 0;

	for (int i = 0; i < MAX_IMG_SIZE; i++) {
		if (i % MAX_IMG_HEIGTH == 0) {
			temp++;
		}
		if (temp % 2 == 0) {
			img_buff[i] = RGB_GREEN;
		} else {
			img_buff[i] = RGB_RED;
		}
	}
}
