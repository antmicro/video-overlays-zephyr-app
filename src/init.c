#include <drivers/dma.h>
#include <stdio.h>
#include "init.h"

void cam1_dma_user_callback(const struct device *dma_dev, void *arg,
			      uint32_t id, int error_code)
{
	switch (cam1_buffer_index)
	{
	case 0:
		dma_block_cfg_cam.dest_address = &img_buff_1;
		dma_config(fastvdma_dev_cam_1, 0, &dma_cfg_cam1);
		cam1_buffer_index = 2;
		break;
	case 1:
		dma_block_cfg_cam.dest_address = &img_buff_2;
		dma_config(fastvdma_dev_cam_1, 0, &dma_cfg_cam1);
		cam1_buffer_index = 0;
		break;
	case 2:
		dma_block_cfg_cam.dest_address = &img_buff_3;
		dma_config(fastvdma_dev_cam_1, 0, &dma_cfg_cam1);
		cam1_buffer_index = 1;
		break;
	default:
		break;
	}
}

void cam1_with_gpu_dma_user_callback(const struct device *dma_dev, void *arg,
			      uint32_t id, int error_code)
{
	switch (cam1_buffer_index)
	{
	case 0:
		dma_block_cfg_cam.dest_address = &img_buff_1;
		dma_config(fastvdma_dev_cam_1, 0, &dma_cfg_cam1);
		cam1_buffer_index = 2;
		blend_images(image_with_text, img_buff_2, img_buff_7);
		gpu_buffer_index = 2;
		break;
	case 1:
		dma_block_cfg_cam.dest_address = &img_buff_2;
		dma_config(fastvdma_dev_cam_1, 0, &dma_cfg_cam1);
		cam1_buffer_index = 0;
		blend_images(image_with_text, img_buff_3, img_buff_8);
		gpu_buffer_index = 0;
		break;
	case 2:
		dma_block_cfg_cam.dest_address = &img_buff_3;
		dma_config(fastvdma_dev_cam_1, 0, &dma_cfg_cam1);
		cam1_buffer_index = 1;
		blend_images(image_with_text, img_buff_1, img_buff_9);
		gpu_buffer_index = 1;
		break;
	default:
		break;
	}
}

void cam2_dma_user_callback(const struct device *dma_dev, void *arg,
			      uint32_t id, int error_code)
{
	switch (cam2_buffer_index)
	{
	case 0:
		dma_block_cfg_cam.dest_address = &img_buff_4;
		dma_config(fastvdma_dev_cam_2, 0, &dma_cfg_cam2);
		cam2_buffer_index = 2;
		break;
	case 1:
		dma_block_cfg_cam.dest_address = &img_buff_5;
		dma_config(fastvdma_dev_cam_2, 0, &dma_cfg_cam2);
		cam2_buffer_index = 0;
		break;
	case 2:
		dma_block_cfg_cam.dest_address = &img_buff_6;
		dma_config(fastvdma_dev_cam_2, 0, &dma_cfg_cam2);
		cam2_buffer_index = 1;
		break;
	default:
		break;
	}
}

void cam2_with_gpu_dma_user_callback(const struct device *dma_dev, void *arg,
			      uint32_t id, int error_code)
{
	switch (cam2_buffer_index)
	{
	case 0:
		dma_block_cfg_cam.dest_address = &img_buff_4;
		dma_config(fastvdma_dev_cam_2, 0, &dma_cfg_cam2);
		cam2_buffer_index = 2;
		blend_images(image_with_text, img_buff_5, img_buff_7);
		gpu_buffer_index = 2;
		break;
	case 1:
		dma_block_cfg_cam.dest_address = &img_buff_5;
		dma_config(fastvdma_dev_cam_2, 0, &dma_cfg_cam2);
		cam2_buffer_index = 0;
		blend_images(image_with_text, img_buff_6, img_buff_8);
		gpu_buffer_index = 0;
		break;
	case 2:
		dma_block_cfg_cam.dest_address = &img_buff_6;
		dma_config(fastvdma_dev_cam_2, 0, &dma_cfg_cam2);
		cam2_buffer_index = 1;
		blend_images(image_with_text, img_buff_4, img_buff_9);
		gpu_buffer_index = 1;
		break;
	default:
		break;
	}
}

void gpu_in_dma_user_callback(const struct device *dma_dev, void *arg,
			      uint32_t id, int error_code)
{
	// printf("GPU input transfer finished\n");
}

void gpu_out_dma_user_callback(const struct device *dma_dev, void *arg,
			      uint32_t id, int error_code)
{
	// printf("GPU output transfer finished\n");
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

	/* enable loop mode */
	dma_block_cfg_cam.source_gather_en = 1;
	dma_block_cfg_cam.dest_scatter_en = 1;

	/* set image height */
	dma_block_cfg_cam.source_gather_count = 600;
	dma_block_cfg_cam.dest_scatter_count = 600;

	/*set block size, driver will get image width from that */
	dma_block_cfg_cam.block_size = 800 * 600;

	dma_block_cfg_cam.dest_address = (uint32_t)&img_buff_2;
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
	dma_block_cfg_gpu_in.source_gather_en = 1;
	dma_block_cfg_gpu_in.dest_scatter_en = 1;

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
	dma_block_cfg_gpu_out.source_gather_en = 1;
	dma_block_cfg_gpu_out.dest_scatter_en = 1;

	/* set image height */
	dma_block_cfg_gpu_out.source_gather_count = 600;
	dma_block_cfg_gpu_out.dest_scatter_count = 600;

	/*set block size, driver will get image width from that */
	dma_block_cfg_gpu_out.block_size = 800 * 600;

	dma_config(fastvdma_dev_gpu_out, 0, &dma_cfg_gpu_out);
}
