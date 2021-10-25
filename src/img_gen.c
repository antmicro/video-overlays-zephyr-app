#include <zephyr.h>
#include "font.h"

void generate_image_with_text(uint32_t image_with_text[], char* text)
{
	int width = 800;
	int word_len = strlen(text);
	
	/* To choose an X-th row, do: X * row_size 
	   To choose an Y-th col, do: Y
	   To write at [15][30], do: [15 * row_size + 30]
	*/
	for(int k = 0; k < word_len; k++) {
		int j = 0;
		int character = text[k] - ' ';
		for(int i = 12; i >= 0; i --) {
			if(width < 480) {
				if ((font[character][i] >> 7) & 1) {
					image_with_text[j * width + (k * 13) + 0] = 0xff000000;
				} 
				if ((font[character][i] >> 6) & 1) {
					image_with_text[j * width + (k * 13) + 1] = 0xff000000;
				} 
				if ((font[character][i] >> 5) & 1) {
					image_with_text[j * width + (k * 13) + 2] = 0xff000000;
				} 
				if ((font[character][i] >> 4) & 1) {
					image_with_text[j * width + (k * 13) + 3] = 0xff000000;
				} 
				if ((font[character][i] >> 3) & 1) {
					image_with_text[j * width + (k * 13) + 4] = 0xff000000;
				} 
				if ((font[character][i] >> 2) & 1) {
					image_with_text[j * width + (k * 13) + 5] = 0xff000000;
				} 
				if ((font[character][i] >> 1) & 1) {
					image_with_text[j * width + (k * 13) + 6] = 0xff000000;
				} 
				if ((font[character][i] >> 0) & 1) {
					image_with_text[j * width + (k * 13) + 7] = 0xff000000;
				}
				j++;
			} else {
				if ((font[character][i] >> 7) & 1) {
					image_with_text[(j + 0) * width + (k * 13 * 2) + 0] = 0xff000000;
					image_with_text[(j + 0) * width + (k * 13 * 2) + 1] = 0xff000000;
					image_with_text[(j + 1) * width + (k * 13 * 2) + 0] = 0xff000000;
					image_with_text[(j + 1) * width + (k * 13 * 2) + 1] = 0xff000000;
				}
				if ((font[character][i] >> 6) & 1) {
					image_with_text[(j + 0) * width + (k * 13 * 2) + 2] = 0xff000000;
					image_with_text[(j + 0) * width + (k * 13 * 2) + 3] = 0xff000000;
					image_with_text[(j + 1) * width + (k * 13 * 2) + 2] = 0xff000000;
					image_with_text[(j + 1) * width + (k * 13 * 2) + 3] = 0xff000000;
				}
				if ((font[character][i] >> 5) & 1) {
					image_with_text[(j + 0) * width + (k * 13 * 2) + 4] = 0xff000000;
					image_with_text[(j + 0) * width + (k * 13 * 2) + 5] = 0xff000000;
					image_with_text[(j + 1) * width + (k * 13 * 2) + 4] = 0xff000000;
					image_with_text[(j + 1) * width + (k * 13 * 2) + 5] = 0xff000000;
				}
				if ((font[character][i] >> 4) & 1) {
					image_with_text[(j + 0) * width + (k * 13 * 2) + 6] = 0xff000000;
					image_with_text[(j + 0) * width + (k * 13 * 2) + 7] = 0xff000000;
					image_with_text[(j + 1) * width + (k * 13 * 2) + 6] = 0xff000000;
					image_with_text[(j + 1) * width + (k * 13 * 2) + 7] = 0xff000000;
				}
				if ((font[character][i] >> 3) & 1) {
					image_with_text[(j + 0) * width + (k * 13 * 2) + 8] = 0xff000000;
					image_with_text[(j + 0) * width + (k * 13 * 2) + 9] = 0xff000000;
					image_with_text[(j + 1) * width + (k * 13 * 2) + 8] = 0xff000000;
					image_with_text[(j + 1) * width + (k * 13 * 2) + 9] = 0xff000000;
				}
				if ((font[character][i] >> 2) & 1) {
					image_with_text[(j + 0) * width + (k * 13 * 2) + 10] = 0xff000000;
					image_with_text[(j + 0) * width + (k * 13 * 2) + 11] = 0xff000000;
					image_with_text[(j + 1) * width + (k * 13 * 2) + 10] = 0xff000000;
					image_with_text[(j + 1) * width + (k * 13 * 2) + 11] = 0xff000000;
				}
				if ((font[character][i] >> 1) & 1) {
					image_with_text[(j + 0) * width + (k * 13 * 2) + 12] = 0xff000000;
					image_with_text[(j + 0) * width + (k * 13 * 2) + 13] = 0xff000000;
					image_with_text[(j + 1) * width + (k * 13 * 2) + 12] = 0xff000000;
					image_with_text[(j + 1) * width + (k * 13 * 2) + 13] = 0xff000000;
				}
				if ((font[character][i] >> 0) & 1) {
					image_with_text[(j + 0) * width + (k * 13 * 2) + 14] = 0xff000000;
					image_with_text[(j + 0) * width + (k * 13 * 2) + 15] = 0xff000000;
					image_with_text[(j + 1) * width + (k * 13 * 2) + 14] = 0xff000000;
					image_with_text[(j + 1) * width + (k * 13 * 2) + 15] = 0xff000000;
				}
				j += 2;
			}
		}
	}
}
