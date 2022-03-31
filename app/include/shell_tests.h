/*
 * Copyright (c) 2021-2022 Antmicro
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "img_gen.h"
#include "gpu.h"
#include "init.h"

extern const struct device *ov2640_dev_1;
extern const struct device *ov2640_dev_2;
extern uint32_t *hdmi_buffers[3];
extern uint32_t *hdmi_buffers_overlay[3];
extern uint32_t img_length_1;
extern uint32_t img_length_2;
extern struct video_format fmt_1;
extern struct video_format fmt_2;
extern struct video_caps caps;
extern uint32_t logo_offset_x;
extern uint32_t logo_offset_y;

extern int cam_buffer_index;
extern int gpu_buffer_index;

extern const k_tid_t hdmi_id;
extern const k_tid_t gpu_id;
extern const k_tid_t cam_id;

extern bool suspend_hdmi;
extern bool suspend_cam;
extern bool suspend_gpu;

extern enum mode{cams, overlay} mode;
extern enum mode mode;

extern char *overlay_text;

extern struct k_sem my_sem;

extern enum overlay_mode {text, logo} overlay_mode;
extern enum overlay_mode overlay_mode;

#define CHAR_CAN 0x18
#define CHAR_DC1 0x11

void flush_l2_cache(void);
void blend_images(uint32_t read_addr, uint32_t write_addr);
