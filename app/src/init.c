/*
 * Copyright (c) 2021-2022 Antmicro
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <drivers/dma.h>
#include <stdio.h>
#include <kernel.h>
#include "init.h"
#include "shell_tests.h"
#include <timing/timing.h>

int hdmi_buffer_index;
int block_buff[3] = { 0 };
int blocked_buff_gpu;
int blocked_buff_cam;
bool callback_cam_block;
bool callback_gpu_block;

void cam1_dma_user_callback(const struct device *dma_dev, void *arg,
				  uint32_t id, int error_code)
{
#ifdef MEASURE_PERFORMANCE
	end_time_cam = timing_buffer_index_get();

	if (n_measure_cam <= 100) {
		measures_cam[n_measure_cam] = (timing_cycles_to_ns(timing_cycles_get(&start_time_cam, &end_time_cam)) / 1000000);
		n_measure_cam++;
	}
#endif

	dma_block_cfg_cam.dest_address = (uint32_t)hdmi_buffers[cam_buffer_index];
	dma_config(fastvdma_dev_cam_1, 0, &dma_cfg_cam1);

	if (cam_buffer_index == 0) {
		cam_buffer_index = 2;
	} else if (cam_buffer_index == 1) {
		cam_buffer_index = 0;
	} else if (cam_buffer_index == 2) {
		cam_buffer_index = 1;
	}

#ifdef MEASURE_PERFORMANCE
	start_time_cam = timing_buffer_index_get();
#endif

}

void cam_with_gpu_dma_user_callback(const struct device *dma_dev, void *arg,
				  uint32_t id, int error_code)
{
#ifdef MEASURE_PERFORMANCE
	end_time_cam = timing_buffer_index_get();

	if (n_measure_cam <= 100) {
		measures_cam[n_measure_cam] = (timing_cycles_to_ns(timing_cycles_get(&start_time_cam, &end_time_cam)) / 1000000);
		n_measure_cam++;
	}
#endif

	callback_cam_block = true;

#ifdef MEASURE_PERFORMANCE
	start_time_cam = timing_buffer_index_get();
#endif
}

void cam2_dma_user_callback(const struct device *dma_dev, void *arg,
				  uint32_t id, int error_code)
{
#ifdef MEASURE_PERFORMANCE
	end_time_cam = timing_buffer_index_get();

	if (n_measure_cam <= 100) {
		measures_cam[n_measure_cam] = (timing_cycles_to_ns(timing_cycles_get(&start_time_cam, &end_time_cam)) / 1000000);
		n_measure_cam++;
	}
#endif

	dma_block_cfg_cam.dest_address = (uint32_t)hdmi_buffers[cam_buffer_index];
	dma_config(fastvdma_dev_cam_2, 0, &dma_cfg_cam2);

	if (cam_buffer_index == 0) {
		cam_buffer_index = 2;
	} else if (cam_buffer_index == 1) {
		cam_buffer_index = 0;
	} else if (cam_buffer_index == 2) {
		cam_buffer_index = 1;
	}

#ifdef MEASURE_PERFORMANCE
	start_time_cam = timing_buffer_index_get();
#endif

}

void gpu_in_dma_user_callback(const struct device *dma_dev, void *arg,
				  uint32_t id, int error_code)
{

}

void gpu_out_dma_user_callback(const struct device *dma_dev, void *arg,
				  uint32_t id, int error_code)
{
#ifdef MEASURE_PERFORMANCE
	end_time_gpu = timing_buffer_index_get();

	if (n_measure_gpu <= 100) {
		measures_gpu[n_measure_gpu] = (timing_cycles_to_ns(timing_cycles_get(&start_time_gpu, &end_time_gpu)) / 1000000);
		n_measure_gpu++;
	}
#endif

	callback_gpu_block = true;
}

void dma_init_cam1(void)
{
	dma_cfg_cam1.channel_direction = PERIPHERAL_TO_MEMORY;
	dma_cfg_cam1.head_block = &dma_block_cfg_cam;
	dma_cfg_cam1.dma_callback = cam1_dma_user_callback;
	dma_cfg_cam1.user_data = NULL;

	/* disable not connected sync signal on writer */
	dma_cfg_cam1.dest_handshake = 1;

	/* from peripheral to memory (0) */
	dma_block_cfg_cam.source_address = 0;

	/* enable loop mode */
	dma_block_cfg_cam.source_gather_en = 1;
	dma_block_cfg_cam.dest_scatter_en = 1;

	/* set image height */
	dma_block_cfg_cam.source_gather_count = 600;
	dma_block_cfg_cam.dest_scatter_count = 600;

	/*set block size, driver will get image width from that */
	dma_block_cfg_cam.block_size = 800 * 600;

	dma_block_cfg_cam.dest_address = (uint32_t)&cam_buff_1;
	dma_config(fastvdma_dev_cam_1, 0, &dma_cfg_cam1);

}

void dma_init_cam2(void)
{
	dma_cfg_cam2.channel_direction = PERIPHERAL_TO_MEMORY;
	dma_cfg_cam2.head_block = &dma_block_cfg_cam;
	dma_cfg_cam2.dma_callback = cam2_dma_user_callback;
	dma_cfg_cam2.user_data = NULL;

	/* disable not connected sync signal on writer */
	dma_cfg_cam2.dest_handshake = 1;

	/* from peripheral to memory (0) */
	dma_block_cfg_cam.source_address = 0;

	/* enable loop mode */
	dma_block_cfg_cam.source_gather_en = 1;
	dma_block_cfg_cam.dest_scatter_en = 1;

	/* set image height */
	dma_block_cfg_cam.source_gather_count = 600;
	dma_block_cfg_cam.dest_scatter_count = 600;

	/*set block size, driver will get image width from that */
	dma_block_cfg_cam.block_size = 800 * 600;

	dma_block_cfg_cam.dest_address = (uint32_t)&cam_buff_1;
	dma_config(fastvdma_dev_cam_2, 0, &dma_cfg_cam2);
}

void dma_init_gpu_inputs(void)
{
	dma_cfg_gpu_in.channel_direction = MEMORY_TO_PERIPHERAL;
	dma_cfg_gpu_in.head_block = &dma_block_cfg_gpu_in;
	dma_cfg_gpu_in.dma_callback = gpu_in_dma_user_callback;
	dma_cfg_gpu_in.user_data = NULL;

	/* disable not connected sync signals */
	dma_cfg_gpu_in.source_handshake = 1;
	dma_cfg_gpu_in.dest_handshake = 1;

	/* from memory to peripheral */
	dma_block_cfg_gpu_in.dest_address = 0;

	/* set image height */
	dma_block_cfg_gpu_in.source_gather_count = 600;
	dma_block_cfg_gpu_in.dest_scatter_count = 600;

	/*set block size, driver will get image width from that */
	dma_block_cfg_gpu_in.block_size = 800 * 600;

	dma_config(fastvdma_dev_gpu_in_1, 0, &dma_cfg_gpu_in);
	dma_config(fastvdma_dev_gpu_in_2, 0, &dma_cfg_gpu_in);
}

void dma_init_gpu_output(void)
{
	dma_cfg_gpu_out.channel_direction = PERIPHERAL_TO_MEMORY;
	dma_cfg_gpu_out.head_block = &dma_block_cfg_gpu_out;
	dma_cfg_gpu_out.dma_callback = gpu_out_dma_user_callback;
	dma_cfg_gpu_out.user_data = NULL;

	/* disable not connected sync signals */
	dma_cfg_gpu_out.source_handshake = 1;
	dma_cfg_gpu_out.dest_handshake = 1;

	/* from peripheral to memory (0) */
	dma_block_cfg_gpu_out.source_address = 0;

	/* set image height */
	dma_block_cfg_gpu_out.source_gather_count = 600;
	dma_block_cfg_gpu_out.dest_scatter_count = 600;

	/*set block size, driver will get image width from that */
	dma_block_cfg_gpu_out.block_size = 800 * 600;

	dma_config(fastvdma_dev_gpu_out, 0, &dma_cfg_gpu_out);
}

void hdmi(void)
{
	while (true) {
		hdmi_out0_core_initiator_enable_write(1);
		switch (mode) {
		case cams:
			hdmi_out0_core_initiator_base_write(
				(uint32_t)hdmi_buffers[cam_buffer_index]);
			break;
		case overlay:
			hdmi_out0_core_initiator_base_write(
				(uint32_t)hdmi_buffers_overlay[hdmi_buffer_index]);
			break;
		default:
			break;
		}
		if (suspend_hdmi) {
			k_thread_suspend(hdmi_id);
		}
		k_msleep(10);
	}
}

void cam(void)
{
	k_sem_give(&my_sem);
	while (true) {
		if (callback_cam_block == true) {
			if (k_sem_take(&my_sem, K_MSEC(10)) != 0) {
				printf("Input data not available- %s!\n", __func__);
			} else {
				for (int i = 0; i < 3; i++) {
					if (!block_buff[i]) {
						dma_block_cfg_cam.dest_address = (uint32_t)hdmi_buffers[i];
						block_buff[i] = true;
						block_buff[blocked_buff_cam] = false;
						gpu_buffer_index = blocked_buff_cam;
						k_sem_give(&my_sem);
						blocked_buff_cam = i;
						break;
					}
				}
				dma_config(fastvdma_dev_cam_1, 0, &dma_cfg_cam1);
				dma_config(fastvdma_dev_cam_2, 0, &dma_cfg_cam2);
			}
			callback_cam_block = false;
		}
		if (suspend_cam) {
			k_thread_suspend(cam_id);
		}
		k_msleep(10);
	}
}

void gpu(void)
{
	int buffer_index = 0;
	int last_buffer_index = 1;

	while (true) {
		if (callback_gpu_block == true) {
			if (k_sem_take(&my_sem, K_FOREVER) != 0) {
				printf("Input data not available- %s!\n", __func__);
			} else {
				hdmi_buffer_index = buffer_index;
				block_buff[blocked_buff_gpu] = false;
				block_buff[gpu_buffer_index] = true;
				blocked_buff_gpu = gpu_buffer_index;
				k_sem_give(&my_sem);
				if (buffer_index == 0)
					last_buffer_index = 1;
				if (buffer_index == 1)
					last_buffer_index = 2;
				if (buffer_index == 2)
					last_buffer_index = 0;
				blend_images((uint32_t)hdmi_buffers[gpu_buffer_index], (uint32_t)hdmi_buffers_overlay[last_buffer_index]);
			}
			callback_gpu_block = false;
			buffer_index++;
			if (buffer_index == 3) {
				buffer_index = 0;
			}
		}
		if (suspend_gpu) {
			callback_gpu_block = false;
			k_thread_suspend(gpu_id);
		}
		k_msleep(10);
	}
}

void suspend_threads(void)
{
	k_thread_suspend(hdmi_id);
	k_thread_suspend(cam_id);
	k_thread_suspend(gpu_id);
}
