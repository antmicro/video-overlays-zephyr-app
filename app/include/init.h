/*
 * Copyright (c) 2021-2022 Antmicro
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "common.h"

extern int cam_buffer_index;
extern int gpu_buffer_index;

extern int block_buff[3];
extern int blocked_buff_gpu;
extern int blocked_buff_cam;
extern bool callback_cam_block;
extern bool callback_gpu_block;

extern const k_tid_t cam_id;
extern const k_tid_t gpu_id;
extern const k_tid_t hdmi_id;

extern bool suspend_hdmi;
extern bool suspend_cam;
extern bool suspend_gpu;

#ifdef MEASURE_PERFORMANCE
extern timing_t start_time_cam, end_time_cam;
extern timing_t start_time_gpu, end_time_gpu;
extern uint64_t measures_cam[100];
extern int n_measure_cam;
extern uint64_t measures_gpu[100];
extern int n_measure_gpu;
#endif

extern enum mode mode;
extern struct k_sem my_sem;

extern enum overlay_mode overlay_mode;

void cam1_dma_user_callback(const struct device *dma_dev, void *arg, uint32_t id, int error_code);
void cam_with_gpu_dma_user_callback(const struct device *dma_dev, void *arg, uint32_t id, int error_code);
void cam2_dma_user_callback(const struct device *dma_dev, void *arg, uint32_t id, int error_code);
void gpu_in_dma_user_callback(const struct device *dma_dev, void *arg, uint32_t id, int error_code);
void gpu_out_dma_user_callback(const struct device *dma_dev, void *arg, uint32_t id, int error_code);
void dma_init_cam1(void);
void dma_init_cam2(void);
void dma_init_gpu_inputs(void);
void dma_init_gpu_output(void);
void hdmi(void);
void cam(void);
void gpu(void);
void suspend_threads(void);
void hdmi_out0_core_initiator_base_write(uint32_t v);
void hdmi_out0_core_initiator_enable_write(uint8_t v);
