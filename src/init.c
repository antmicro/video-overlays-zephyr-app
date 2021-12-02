#include <drivers/dma.h>

extern const struct device* fastvdma_dev_cam_1;
extern const struct device* fastvdma_dev_cam_2;
extern const struct device* fastvdma_dev_gpu_in_1;
extern const struct device* fastvdma_dev_gpu_in_2;
extern const struct device* fastvdma_dev_gpu_out;
extern struct dma_config dma_cfg;
extern struct dma_block_config dma_block_cfg;
extern uint32_t img_buff_1[800 * 600];
extern uint32_t img_buff_2[800 * 600];

void cam_dma_user_callback(const struct device *dma_dev, void *arg,
			      uint32_t id, int error_code)
{
	printf("Capture finished\n");
}

void gpu_in_dma_user_callback(const struct device *dma_dev, void *arg,
			      uint32_t id, int error_code)
{
	printf("GPU input transfer finished\n");
}

void gpu_out_dma_user_callback(const struct device *dma_dev, void *arg,
			      uint32_t id, int error_code)
{
	printf("GPU output transfer finished\n");
}

void dma_init_cams() {
	dma_cfg.channel_direction = PERIPHERAL_TO_MEMORY;
	dma_cfg.head_block = &dma_block_cfg;
	dma_cfg.dma_callback = cam_dma_user_callback;
	dma_cfg.user_data = NULL;

	/* from peripheral to memory (0) */
	dma_block_cfg.source_address = 0;

	/* set image height */
	dma_block_cfg.source_gather_count = 600;
	dma_block_cfg.dest_scatter_count = 600;

	/*set block size, driver will get image width from that */
	dma_block_cfg.block_size = 800 * 600;

	dma_block_cfg.dest_address = &img_buff_1;
	dma_config(fastvdma_dev_cam_1, 0, &dma_cfg);
	dma_block_cfg.dest_address = &img_buff_2;
	dma_config(fastvdma_dev_cam_2, 0, &dma_cfg);
}

void dma_init_gpu_inputs() {
	dma_cfg.channel_direction = MEMORY_TO_PERIPHERAL;
	dma_cfg.head_block = &dma_block_cfg;
	dma_cfg.dma_callback = gpu_in_dma_user_callback;
	dma_cfg.user_data = NULL;

	/* set image height */
	dma_block_cfg.source_gather_count = 600;
	dma_block_cfg.dest_scatter_count = 600;

	/*set block size, driver will get image width from that */
	dma_block_cfg.block_size = 800 * 600;

	dma_config(fastvdma_dev_gpu_in_1, 0, &dma_cfg);
	dma_config(fastvdma_dev_gpu_in_2, 0, &dma_cfg);
}

void dma_init_gpu_output() {
	dma_cfg.channel_direction = PERIPHERAL_TO_MEMORY;
	dma_cfg.head_block = &dma_block_cfg;
	dma_cfg.dma_callback = gpu_out_dma_user_callback;
	dma_cfg.user_data = NULL;

	/* from peripheral to memory (0) */
	dma_block_cfg.source_address = 0;

	/* set image height */
	dma_block_cfg.source_gather_count = 600;
	dma_block_cfg.dest_scatter_count = 600;

	/*set block size, driver will get image width from that */
	dma_block_cfg.block_size = 800 * 600;

	dma_config(fastvdma_dev_gpu_out, 0, &dma_cfg);
}
