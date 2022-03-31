/*
 * Copyright (c) 2021-2022 Antmicro
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define MAX_IMG_SIZE  (1280 * 1024)
#define MAX_IMG_WIDTH  1280
#define MAX_IMG_HEIGTH 1024

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

extern uint32_t overlay_image[MAX_IMG_SIZE];
extern uint32_t cam_buff_1[MAX_IMG_SIZE];
extern uint32_t cam_buff_2[MAX_IMG_SIZE];
extern uint32_t cam_buff_3[MAX_IMG_SIZE];
extern uint32_t gpu_buff_1[MAX_IMG_SIZE];
extern uint32_t gpu_buff_2[MAX_IMG_SIZE];
extern uint32_t gpu_buff_3[MAX_IMG_SIZE];

extern uint32_t *hdmi_buffers[3];
extern uint32_t *hdmi_buffers_overlay[3];
