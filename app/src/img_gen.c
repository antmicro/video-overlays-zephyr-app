/*
 * Copyright (c) 2021-2022 Antmicro
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <string.h>

#include "font.h"
#include "logo_antmicro.h"

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
				if ((font[character][i] >> 7) & 1) {
					overlay_image[v_offset * width + h_offset + (k * 13) + 0] = color;
				}
				if ((font[character][i] >> 6) & 1) {
					overlay_image[v_offset * width + h_offset + (k * 13) + 1] = color;
				}
				if ((font[character][i] >> 5) & 1) {
					overlay_image[v_offset * width + h_offset + (k * 13) + 2] = color;
				}
				if ((font[character][i] >> 4) & 1) {
					overlay_image[v_offset * width + h_offset + (k * 13) + 3] = color;
				}
				if ((font[character][i] >> 3) & 1) {
					overlay_image[v_offset * width + h_offset + (k * 13) + 4] = color;
				}
				if ((font[character][i] >> 2) & 1) {
					overlay_image[v_offset * width + h_offset + (k * 13) + 5] = color;
				}
				if ((font[character][i] >> 1) & 1) {
					overlay_image[v_offset * width + h_offset + (k * 13) + 6] = color;
				}
				if ((font[character][i] >> 0) & 1) {
					overlay_image[v_offset * width + h_offset + (k * 13) + 7] = color;
				}
				v_offset++;
			} else {
				if ((font[character][i] >> 7) & 1) {
					overlay_image[(v_offset + 0) * width + h_offset + (k * 13 * 2) + 0] = color;
					overlay_image[(v_offset + 0) * width + h_offset + (k * 13 * 2) + 1] = color;
					overlay_image[(v_offset + 1) * width + h_offset + (k * 13 * 2) + 0] = color;
					overlay_image[(v_offset + 1) * width + h_offset + (k * 13 * 2) + 1] = color;
				}
				if ((font[character][i] >> 6) & 1) {
					overlay_image[(v_offset + 0) * width + h_offset + (k * 13 * 2) + 2] = color;
					overlay_image[(v_offset + 0) * width + h_offset + (k * 13 * 2) + 3] = color;
					overlay_image[(v_offset + 1) * width + h_offset + (k * 13 * 2) + 2] = color;
					overlay_image[(v_offset + 1) * width + h_offset + (k * 13 * 2) + 3] = color;
				}
				if ((font[character][i] >> 5) & 1) {
					overlay_image[(v_offset + 0) * width + h_offset + (k * 13 * 2) + 4] = color;
					overlay_image[(v_offset + 0) * width + h_offset + (k * 13 * 2) + 5] = color;
					overlay_image[(v_offset + 1) * width + h_offset + (k * 13 * 2) + 4] = color;
					overlay_image[(v_offset + 1) * width + h_offset + (k * 13 * 2) + 5] = color;
				}
				if ((font[character][i] >> 4) & 1) {
					overlay_image[(v_offset + 0) * width + h_offset + (k * 13 * 2) + 6] = color;
					overlay_image[(v_offset + 0) * width + h_offset + (k * 13 * 2) + 7] = color;
					overlay_image[(v_offset + 1) * width + h_offset + (k * 13 * 2) + 6] = color;
					overlay_image[(v_offset + 1) * width + h_offset + (k * 13 * 2) + 7] = color;
				}
				if ((font[character][i] >> 3) & 1) {
					overlay_image[(v_offset + 0) * width + h_offset + (k * 13 * 2) + 8] = color;
					overlay_image[(v_offset + 0) * width + h_offset + (k * 13 * 2) + 9] = color;
					overlay_image[(v_offset + 1) * width + h_offset + (k * 13 * 2) + 8] = color;
					overlay_image[(v_offset + 1) * width + h_offset + (k * 13 * 2) + 9] = color;
				}
				if ((font[character][i] >> 2) & 1) {
					overlay_image[(v_offset + 0) * width + h_offset + (k * 13 * 2) + 10] = color;
					overlay_image[(v_offset + 0) * width + h_offset + (k * 13 * 2) + 11] = color;
					overlay_image[(v_offset + 1) * width + h_offset + (k * 13 * 2) + 10] = color;
					overlay_image[(v_offset + 1) * width + h_offset + (k * 13 * 2) + 11] = color;
				}
				if ((font[character][i] >> 1) & 1) {
					overlay_image[(v_offset + 0) * width + h_offset + (k * 13 * 2) + 12] = color;
					overlay_image[(v_offset + 0) * width + h_offset + (k * 13 * 2) + 13] = color;
					overlay_image[(v_offset + 1) * width + h_offset + (k * 13 * 2) + 12] = color;
					overlay_image[(v_offset + 1) * width + h_offset + (k * 13 * 2) + 13] = color;
				}
				if ((font[character][i] >> 0) & 1) {
					overlay_image[(v_offset + 0) * width + h_offset + (k * 13 * 2) + 14] = color;
					overlay_image[(v_offset + 0) * width + h_offset + (k * 13 * 2) + 15] = color;
					overlay_image[(v_offset + 1) * width + h_offset + (k * 13 * 2) + 14] = color;
					overlay_image[(v_offset + 1) * width + h_offset + (k * 13 * 2) + 15] = color;
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