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

void draw_pattern(int h_active, int v_active)
{
	volatile unsigned int *buffer = (unsigned int *)0x40200000;
	int i;
	int color;
	color = -1;
	/* color bar pattern */
	for (i = 0; i < h_active * v_active; i++) {
		if (i % (h_active / 32) == 0)
			color = inc_color(color);
		if (color >= 0)
			buffer[i] = color_bar[color];
	}
}

void draw_color(int h_active, int v_active, int color)
{
	volatile unsigned int *buffer = (unsigned int *)0x40200000;
	/* color bar pattern */
	for (int i = 0; i < h_active * v_active; i++) {
			buffer[i] = color;
	}
}

void draw_color2(int h_active, int v_active, int color)
{
	volatile unsigned int *buffer = (unsigned int *)0x40F00000;
	/* color bar pattern */
	for (int i = 0; i < h_active * v_active; i++) {
			buffer[i] = color;
	}
}

void draw_image(int h_active, int v_active)
{
	volatile unsigned int *buffer = (unsigned int *)0x40200000;
	/* color bar pattern */
	int temp = 0;
	for (int i = 0; i < h_active * v_active; i++) {
			if(i % h_active == 0){
				temp++;
			}
			if(temp % 2 == 0){
				buffer[i] = RGB_GREEN;
			}
			else{
				buffer[i] = RGB_RED;
			}
	}
}
