#include <drivers/dma.h>

extern const struct device* fastvdma_dev_cam_1;
extern const struct device* fastvdma_dev_cam_2;
extern const struct device* fastvdma_dev_gpu_in_1;
extern const struct device* fastvdma_dev_gpu_in_2;
extern const struct device* fastvdma_dev_gpu_out;
extern const struct device* litevideo_dev;
extern struct dma_config dma_cfg_cam;
extern struct dma_config dma_cfg_gpu_in;
extern struct dma_config dma_cfg_gpu_out;
extern struct dma_config dma_cfg_litevideo;
extern struct dma_block_config dma_block_cfg_cam;
extern struct dma_block_config dma_block_cfg_gpu_in;
extern struct dma_block_config dma_block_cfg_gpu_out;
extern struct dma_block_config dma_block_cfg_litevideo;
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

void litevideo_dma_user_callback(const struct device *dma_dev, void *arg,
			      uint32_t id, int error_code)
{
	printf("HDMI transfer finished\n");
}

void litevideo_dma_init()
{
	dma_cfg_litevideo.dma_callback = litevideo_dma_user_callback;
	dma_cfg_litevideo.user_data = NULL;

	dma_config(litevideo_dev, 0, &dma_cfg_litevideo);
}

void dma_init_cams() {
	dma_cfg_cam.channel_direction = PERIPHERAL_TO_MEMORY;
	dma_cfg_cam.head_block = &dma_block_cfg_cam;
	dma_cfg_cam.dma_callback = cam_dma_user_callback;
	dma_cfg_cam.user_data = NULL;

	/* disable not connected sync signal on writer */
	dma_cfg_cam.dest_handshake = 1;

	/* from peripheral to memory (0) */
	dma_block_cfg_cam.source_address = 0;

	/* set image height */
	dma_block_cfg_cam.source_gather_count = 600;
	dma_block_cfg_cam.dest_scatter_count = 600;

	/*set block size, driver will get image width from that */
	dma_block_cfg_cam.block_size = 800 * 600;

	dma_block_cfg_cam.dest_address = &img_buff_1;
	dma_config(fastvdma_dev_cam_1, 0, &dma_cfg_cam);
	dma_block_cfg_cam.dest_address = &img_buff_2;
	dma_config(fastvdma_dev_cam_2, 0, &dma_cfg_cam);
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

	/* set image height */
	dma_block_cfg_gpu_out.source_gather_count = 600;
	dma_block_cfg_gpu_out.dest_scatter_count = 600;

	/*set block size, driver will get image width from that */
	dma_block_cfg_gpu_out.block_size = 800 * 600;

	dma_config(fastvdma_dev_gpu_out, 0, &dma_cfg_gpu_out);
}
