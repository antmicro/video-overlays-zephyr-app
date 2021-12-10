void cam_dma_user_callback(const struct device *dma_dev, void *arg,
			      uint32_t id, int error_code);
void gpu_in_dma_user_callback(const struct device *dma_dev, void *arg,
			      uint32_t id, int error_code);
void gpu_out_dma_user_callback(const struct device *dma_dev, void *arg,
			      uint32_t id, int error_code);
void litevideo_dma_user_callback(const struct device *dma_dev, void *arg,
			      uint32_t id, int error_code);
void dma_init_cams();
void dma_init_gpu_inputs();
void dma_init_gpu_output();
void litevideo_dma_init();
