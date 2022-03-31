/*
 * Copyright (c) 2021-2022 Antmicro
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <string.h>

#include "font.h"
#include "logo_antmicro.h"

#define CHAR_WIDTH_INDEX 7

extern char overlay_text[25];

void generate_image_with_text(uint32_t overlay_image[], int width, int height)
{
	int color = 0xff000000; // black
	int text_len = strlen(overlay_text);

	for (int i = 0; i < width * height; i++) {
		overlay_image[i] = 0;
	}

	for (int k = 0; k < text_len; k++) {
		int v_offset;
		int h_offset = 10;
		int character = overlay_text[k] - ' ';

		if (width < 480) {
			v_offset = height - 30;
		} else {
			v_offset = height - 70;
		}

		/* Write at [v_offset * width][h_offset + char_offset + char_bit] */
		for(int i = 0; i <= 15; i++) {
			if (width < 480) {
				for (int j = 0; j < CHAR_WIDTH_INDEX; j++) {
					if ((font[character][i] >> (CHAR_WIDTH_INDEX - j)) & 1) {
						overlay_image[v_offset * width + h_offset + (k * 13) + j] = color;
					}
				}
				v_offset++;
			} else {
				for (int j = 0; j < CHAR_WIDTH_INDEX; j++) {
					if ((font[character][i] >> (CHAR_WIDTH_INDEX - j)) & 1) {
						overlay_image[(v_offset + 0) * width + h_offset + (k * 13 * 2) + (2 * j)] = color;
						overlay_image[(v_offset + 0) * width + h_offset + (k * 13 * 2) + ((2 * j) + 1)] = color;
						overlay_image[(v_offset + 1) * width + h_offset + (k * 13 * 2) + (2 * j)] = color;
						overlay_image[(v_offset + 1) * width + h_offset + (k * 13 * 2) + ((2 * j) + 1)] = color;
					}
				}
				v_offset += 2;
			}
		}
	}
}

int generate_image_with_logo(uint32_t overlay_image[], int width, int height, int offset_x, int offset_y)
{
	if(offset_x + LOGO_WIDTH > width || offset_y + LOGO_HEIGHT > height) {
		return 1;
	}

	for(int i = 0; i < width * height; i++) {
		overlay_image[i] = 0;
	}

	int iter = 0;
	for(int row = 0; row < LOGO_HEIGHT; row++) {
		for(int col = 0; col < LOGO_WIDTH; col++) {
			int pixel = logo_rgba[iter + 3] << 24 |
						logo_rgba[iter + 2] << 16 |
						logo_rgba[iter + 1] << 8  |
						logo_rgba[iter + 0];
			iter += 4;

			overlay_image[((offset_y + row) * width) + (offset_x + col)] = pixel;
		}
	}

	return 0;
}