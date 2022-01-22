#include <drivers/dma.h>
#include <stdio.h>
#include <kernel.h>
#include "init.h"
#include "shell_tests.h"

int hdmi_buffer_index = 0;
int block_buff[3] = { 0 };
int blocked_buff_gpu = 0;
int blocked_buff_cam = 0;
bool callback_cam_block = false;
bool callback_gpu_block = false;

void cam1_dma_user_callback(const struct device *dma_dev, void *arg,
			      uint32_t id, int error_code)
{
	dma_block_cfg_cam.dest_address = (uint32_t)hdmi_buffers[cam_buffer_index];
	dma_config(fastvdma_dev_cam_1, 0, &dma_cfg_cam1);

	if (cam_buffer_index == 0) { 
		cam_buffer_index = 2;
	} else if (cam_buffer_index == 1) {
		cam_buffer_index = 0;
	} else if (cam_buffer_index == 2) {
		cam_buffer_index = 1;
	}
}

void cam_with_gpu_dma_user_callback(const struct device *dma_dev, void *arg,
			      uint32_t id, int error_code)
{
	callback_cam_block = true;
}


void cam2_dma_user_callback(const struct device *dma_dev, void *arg,
			      uint32_t id, int error_code)
{
	dma_block_cfg_cam.dest_address = (uint32_t)hdmi_buffers[cam_buffer_index];
	dma_config(fastvdma_dev_cam_2, 0, &dma_cfg_cam2);

	if (cam_buffer_index == 0) { 
		cam_buffer_index = 2;
	} else if (cam_buffer_index == 1) {
		cam_buffer_index = 0;
	} else if (cam_buffer_index == 2) {
		cam_buffer_index = 1;
	}
}

void gpu_in_dma_user_callback(const struct device *dma_dev, void *arg,
			      uint32_t id, int error_code)
{

}

void gpu_out_dma_user_callback(const struct device *dma_dev, void *arg,
			      uint32_t id, int error_code)
{
	callback_gpu_block = true;
}


void dma_init_cam1() {
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

	dma_block_cfg_cam.dest_address = (uint32_t)&img_buff_1;
	dma_config(fastvdma_dev_cam_1, 0, &dma_cfg_cam1);

}

void dma_init_cam2() {
	dma_cfg_cam2.channel_direction = PERIPHERAL_TO_MEMORY;
	dma_cfg_cam2.head_block = &dma_block_cfg_cam;
	dma_cfg_cam2.dma_callback = cam2_dma_user_callback;
	dma_cfg_cam2.user_data = NULL;

	/* disable not connected sync signal on writer */
	dma_cfg_cam2.dest_handshake = 1;

	/* from peripheral to memory (0) */
	dma_block_cfg_cam.source_address = 0;

	// /* enable loop mode */
	dma_block_cfg_cam.source_gather_en = 1;
	dma_block_cfg_cam.dest_scatter_en = 1;

	/* set image height */
	dma_block_cfg_cam.source_gather_count = 600;
	dma_block_cfg_cam.dest_scatter_count = 600;

	/*set block size, driver will get image width from that */
	dma_block_cfg_cam.block_size = 800 * 600;

	dma_block_cfg_cam.dest_address = (uint32_t)&img_buff_1;
	dma_config(fastvdma_dev_cam_2, 0, &dma_cfg_cam2);
}

void dma_init_gpu_inputs() {
	dma_cfg_gpu_in.channel_direction = MEMORY_TO_PERIPHERAL;
	dma_cfg_gpu_in.head_block = &dma_block_cfg_gpu_in;
	dma_cfg_gpu_in.dma_callback = gpu_in_dma_user_callback;
	dma_cfg_gpu_in.user_data = NULL;

	/* disable not connected sync signals */
	dma_cfg_gpu_in.source_handshake = 1;
	dma_cfg_gpu_in.dest_handshake = 1;

	/* from memory to peripheral */
	dma_block_cfg_gpu_in.dest_address = 0;

	/* enable loop mode */
	// dma_block_cfg_gpu_in.source_gather_en = 1;
	// dma_block_cfg_gpu_in.dest_scatter_en = 1;

	/* set image height */
	dma_block_cfg_gpu_in.source_gather_count = 600;
	dma_block_cfg_gpu_in.dest_scatter_count = 600;

	/*set block size, driver will get image width from that */
	dma_block_cfg_gpu_in.block_size = 800 * 600;

	dma_config(fastvdma_dev_gpu_in_1, 0, &dma_cfg_gpu_in);
	dma_config(fastvdma_dev_gpu_in_2, 0, &dma_cfg_gpu_in);
}

void dma_init_gpu_output() {
	dma_cfg_gpu_out.channel_direction = PERIPHERAL_TO_MEMORY;
	dma_cfg_gpu_out.head_block = &dma_block_cfg_gpu_out;
	dma_cfg_gpu_out.dma_callback = gpu_out_dma_user_callback;
	dma_cfg_gpu_out.user_data = NULL;

	/* disable not connected sync signals */
	dma_cfg_gpu_out.source_handshake = 1;
	dma_cfg_gpu_out.dest_handshake = 1;

	/* from peripheral to memory (0) */
	dma_block_cfg_gpu_out.source_address = 0;

	/* enable loop mode */
	// dma_block_cfg_gpu_out.source_gather_en = 1;
	// dma_block_cfg_gpu_out.dest_scatter_en = 1;

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
				printk("Input data not available- cam!\n");
			} else {
				if (!block_buff[0]) {
					dma_block_cfg_cam.dest_address = (uint32_t)&img_buff_1;
					block_buff[0] = true;
					block_buff[blocked_buff_cam] = false;
					gpu_buffer_index = blocked_buff_cam;
					k_sem_give(&my_sem);
					blocked_buff_cam = 0;
				} else if (!block_buff[1]) {
					dma_block_cfg_cam.dest_address = (uint32_t)&img_buff_2;
					block_buff[1] = true;
					block_buff[blocked_buff_cam] = false;
					gpu_buffer_index = blocked_buff_cam;
					k_sem_give(&my_sem);
					blocked_buff_cam = 1;
				} else if (!block_buff[2]) {
					dma_block_cfg_cam.dest_address = (uint32_t)&img_buff_3;
					block_buff[2] = true;
					block_buff[blocked_buff_cam] = false;
					gpu_buffer_index = blocked_buff_cam;
					k_sem_give(&my_sem);
					blocked_buff_cam = 2;
				}
				dma_config(fastvdma_dev_cam_1, 0, &dma_cfg_cam1);
				dma_config(fastvdma_dev_cam_2, 0, &dma_cfg_cam2);
			}

			callback_cam_block = false;
		}
		if (suspend_cam) {
			callback_cam_block = false;
			k_thread_suspend(cam_id);
		}
		k_msleep(10);
	}
}

void gpu(void)
{
	int counter = 0;
	int counter2 = 1;
	while (true) {
		if (callback_gpu_block == true) {
			if (k_sem_take(&my_sem, K_FOREVER) != 0) {
				printk("Input data not available - gpu!\n");
			} else {
				hdmi_buffer_index = counter;
				block_buff[blocked_buff_gpu] = false;
				block_buff[gpu_buffer_index] = true;
				blocked_buff_gpu = gpu_buffer_index;
				k_sem_give(&my_sem);
				if (counter == 0) counter2 = 1;
				if (counter == 1) counter2 = 2;
				if (counter == 2) counter2 = 0;
				blend_images((uint32_t)&image_with_text,
						(uint32_t)hdmi_buffers[gpu_buffer_index], (uint32_t)hdmi_buffers_overlay[counter2]);
			}
			callback_gpu_block = false;
			counter++;
			if (counter == 3) {
				counter = 0;
			}
		}
		if (suspend_gpu) {
			callback_gpu_block = false;
			k_thread_suspend(gpu_id);
		}
		k_msleep(10);
	}
}

void suspend_threads()
{
	k_thread_suspend(hdmi_id);
	k_thread_suspend(cam_id);
	k_thread_suspend(gpu_id);
}