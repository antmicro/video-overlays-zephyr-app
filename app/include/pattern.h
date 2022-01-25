/*
 * Copyright (c) 2021-2022 Antmicro
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
extern uint32_t img_buff_7[1280 * 1024];
#define RGB_WHITE  0x00ffffff
#define RGB_YELLOW 0x0000ffff
#define RGB_CYAN   0x00ffff00
#define RGB_GREEN  0x0000ff00
#define RGB_PURPLE 0x00ff00ff
#define RGB_RED    0x000000ff
#define RGB_BLUE   0x00ff0000
#define RGB_BLACK  0x00000000
void draw_pattern(int h_active, int v_active);
void draw_color(int h_active, int v_active, int color);
void draw_color2(int h_active, int v_active, int color);
void draw_image(int h_active, int v_active);
