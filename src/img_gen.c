#include <zephyr.h>
#include <string.h>

#include "font.h"

extern char overlay_text[25];

void generate_image_with_text(uint32_t image_with_text[], int width, int height)
{
	int color = 0xff000000; // black
	int text_len = strlen(overlay_text);

	for (int i = 0; i < width * height; i++) {
		image_with_text[i] = 0;
	}

	for(int k = 0; k < text_len; k++) {
		int v_offset;
		int h_offset = 10;
		int character = overlay_text[k] - ' ';

		if(width < 480) {
			v_offset = height - 30;
		} else {
			v_offset = height - 70;
		}

		/* Write at [v_offset * width][h_offset + char_offset + char_bit] */
		for(int i = 12; i >= 0; i --) {
			if(width < 480) {
				if ((font[character][i] >> 7) & 1) {
					image_with_text[v_offset * width + h_offset + (k * 13) + 0] = color;
				}
				if ((font[character][i] >> 6) & 1) {
					image_with_text[v_offset * width + h_offset + (k * 13) + 1] = color;
				}
				if ((font[character][i] >> 5) & 1) {
					image_with_text[v_offset * width + h_offset + (k * 13) + 2] = color;
				}
				if ((font[character][i] >> 4) & 1) {
					image_with_text[v_offset * width + h_offset + (k * 13) + 3] = color;
				}
				if ((font[character][i] >> 3) & 1) {
					image_with_text[v_offset * width + h_offset + (k * 13) + 4] = color;
				}
				if ((font[character][i] >> 2) & 1) {
					image_with_text[v_offset * width + h_offset + (k * 13) + 5] = color;
				}
				if ((font[character][i] >> 1) & 1) {
					image_with_text[v_offset * width + h_offset + (k * 13) + 6] = color;
				}
				if ((font[character][i] >> 0) & 1) {
					image_with_text[v_offset * width + h_offset + (k * 13) + 7] = color;
				}
				v_offset++;
			} else {
				if ((font[character][i] >> 7) & 1) {
					image_with_text[(v_offset + 0) * width + h_offset + (k * 13 * 2) + 0] = color;
					image_with_text[(v_offset + 0) * width + h_offset + (k * 13 * 2) + 1] = color;
					image_with_text[(v_offset + 1) * width + h_offset + (k * 13 * 2) + 0] = color;
					image_with_text[(v_offset + 1) * width + h_offset + (k * 13 * 2) + 1] = color;
				}
				if ((font[character][i] >> 6) & 1) {
					image_with_text[(v_offset + 0) * width + h_offset + (k * 13 * 2) + 2] = color;
					image_with_text[(v_offset + 0) * width + h_offset + (k * 13 * 2) + 3] = color;
					image_with_text[(v_offset + 1) * width + h_offset + (k * 13 * 2) + 2] = color;
					image_with_text[(v_offset + 1) * width + h_offset + (k * 13 * 2) + 3] = color;
				}
				if ((font[character][i] >> 5) & 1) {
					image_with_text[(v_offset + 0) * width + h_offset + (k * 13 * 2) + 4] = color;
					image_with_text[(v_offset + 0) * width + h_offset + (k * 13 * 2) + 5] = color;
					image_with_text[(v_offset + 1) * width + h_offset + (k * 13 * 2) + 4] = color;
					image_with_text[(v_offset + 1) * width + h_offset + (k * 13 * 2) + 5] = color;
				}
				if ((font[character][i] >> 4) & 1) {
					image_with_text[(v_offset + 0) * width + h_offset + (k * 13 * 2) + 6] = color;
					image_with_text[(v_offset + 0) * width + h_offset + (k * 13 * 2) + 7] = color;
					image_with_text[(v_offset + 1) * width + h_offset + (k * 13 * 2) + 6] = color;
					image_with_text[(v_offset + 1) * width + h_offset + (k * 13 * 2) + 7] = color;
				}
				if ((font[character][i] >> 3) & 1) {
					image_with_text[(v_offset + 0) * width + h_offset + (k * 13 * 2) + 8] = color;
					image_with_text[(v_offset + 0) * width + h_offset + (k * 13 * 2) + 9] = color;
					image_with_text[(v_offset + 1) * width + h_offset + (k * 13 * 2) + 8] = color;
					image_with_text[(v_offset + 1) * width + h_offset + (k * 13 * 2) + 9] = color;
				}
				if ((font[character][i] >> 2) & 1) {
					image_with_text[(v_offset + 0) * width + h_offset + (k * 13 * 2) + 10] = color;
					image_with_text[(v_offset + 0) * width + h_offset + (k * 13 * 2) + 11] = color;
					image_with_text[(v_offset + 1) * width + h_offset + (k * 13 * 2) + 10] = color;
					image_with_text[(v_offset + 1) * width + h_offset + (k * 13 * 2) + 11] = color;
				}
				if ((font[character][i] >> 1) & 1) {
					image_with_text[(v_offset + 0) * width + h_offset + (k * 13 * 2) + 12] = color;
					image_with_text[(v_offset + 0) * width + h_offset + (k * 13 * 2) + 13] = color;
					image_with_text[(v_offset + 1) * width + h_offset + (k * 13 * 2) + 12] = color;
					image_with_text[(v_offset + 1) * width + h_offset + (k * 13 * 2) + 13] = color;
				}
				if ((font[character][i] >> 0) & 1) {
					image_with_text[(v_offset + 0) * width + h_offset + (k * 13 * 2) + 14] = color;
					image_with_text[(v_offset + 0) * width + h_offset + (k * 13 * 2) + 15] = color;
					image_with_text[(v_offset + 1) * width + h_offset + (k * 13 * 2) + 14] = color;
					image_with_text[(v_offset + 1) * width + h_offset + (k * 13 * 2) + 15] = color;
				}
				v_offset += 2;
			}
		}
	}
}
