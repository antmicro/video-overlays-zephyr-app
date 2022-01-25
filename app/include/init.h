/*
 * Copyright (c) 2021-2022 Antmicro
 *
 * SPDX-License-Identifier: Apache-2.0
 */

extern const struct device *fastvdma_dev_cam_1;
extern const struct device *fastvdma_dev_cam_2;
extern const struct device *fastvdma_dev_gpu_in_1;
extern const struct device *fastvdma_dev_gpu_in_2;
extern const struct device *fastvdma_dev_gpu_out;
extern struct dma_config dma_cfg_cam1;
extern struct dma_config dma_cfg_cam2;
extern struct dma_config dma_cfg_gpu_in;
extern struct dma_config dma_cfg_gpu_out;
extern struct dma_block_config dma_block_cfg_cam;
extern struct dma_block_config dma_block_cfg_gpu_in;
extern struct dma_block_config dma_block_cfg_gpu_out;
extern uint32_t image_with_text[1280 * 1024];
extern uint32_t img_buff_1[1280 * 1024];
extern uint32_t img_buff_2[1280 * 1024];
extern uint32_t img_buff_3[1280 * 1024];
extern uint32_t img_buff_4[1280 * 1024];
extern uint32_t img_buff_5[1280 * 1024];
extern uint32_t img_buff_6[1280 * 1024];
extern uint32_t img_buff_7[1280 * 1024];
extern uint32_t *hdmi_buffers[3];
extern uint32_t *hdmi_buffers_overlay[3];
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

extern timing_t start_time_cam, end_time_cam;
extern timing_t start_time_gpu, end_time_gpu;
extern uint64_t measures_cam[100];
extern int n_measure_cam;
extern uint64_t measures_gpu[100];
extern int n_measure_gpu;

extern enum mode mode;
extern struct k_sem my_sem;

void cam1_dma_user_callback(const struct device *dma_dev, void *arg, uint32_t id, int error_code);
void cam_with_gpu_dma_user_callback(const struct device *dma_dev, void *arg, uint32_t id, int error_code);
void cam2_dma_user_callback(const struct device *dma_dev, void *arg, uint32_t id, int error_code);
void gpu_in_dma_user_callback(const struct device *dma_dev, void *arg, uint32_t id, int error_code);
void gpu_out_dma_user_callback(const struct device *dma_dev, void *arg, uint32_t id, int error_code);
void dma_init_cam1();
void dma_init_cam2();
void dma_init_gpu_inputs();
void dma_init_gpu_output();
void hdmi(void);
void cam(void);
void gpu(void);
void suspend_threads();
void hdmi_out0_core_initiator_base_write(uint32_t v);
void hdmi_out0_core_initiator_enable_write(uint8_t v);
