/*
 * Copyright (c) 2021-2022 Antmicro
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include "common.h"

#define RGB_WHITE  0x00ffffff
#define RGB_YELLOW 0x0000ffff
#define RGB_CYAN   0x00ffff00
#define RGB_GREEN  0x0000ff00
#define RGB_PURPLE 0x00ff00ff
#define RGB_RED    0x000000ff
#define RGB_BLUE   0x00ff0000
#define RGB_BLACK  0x00000000

void draw_pattern(uint32_t img_buff[]);
void draw_color(uint32_t img_buff[], int color);
void draw_colorbar(uint32_t img_buff[]);
