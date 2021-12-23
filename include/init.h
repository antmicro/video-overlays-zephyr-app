extern const struct device* fastvdma_dev_cam_1;
extern const struct device* fastvdma_dev_cam_2;
extern const struct device* fastvdma_dev_gpu_in_1;
extern const struct device* fastvdma_dev_gpu_in_2;
extern const struct device* fastvdma_dev_gpu_out;
extern struct dma_config dma_cfg_cam1;
extern struct dma_config dma_cfg_cam2;
extern struct dma_config dma_cfg_gpu_in;
extern struct dma_config dma_cfg_gpu_out;
extern struct dma_block_config dma_block_cfg_cam;
extern struct dma_block_config dma_block_cfg_gpu_in;
extern struct dma_block_config dma_block_cfg_gpu_out;
extern uint32_t img_buff_1[800 * 600];
extern uint32_t img_buff_2[800 * 600];
extern uint32_t img_buff_3[800 * 600];
extern uint32_t img_buff_4[800 * 600];
extern uint32_t img_buff_5[800 * 600];
extern uint32_t img_buff_6[800 * 600];
extern uint32_t img_buff_7[800 * 600];
extern uint32_t img_buff_8[800 * 600];
extern uint32_t img_buff_9[800 * 600];
extern uint32_t* hdmi_buffers1[3];
extern uint32_t* hdmi_buffers2[3];
extern uint32_t* hdmi_buffers3[3];
extern int cam1_buffer_index;
extern int cam2_buffer_index;
extern int gpu_buffer_index;

void cam1_dma_user_callback(const struct device *dma_dev, void *arg,
			      uint32_t id, int error_code);
void cam2_dma_user_callback(const struct device *dma_dev, void *arg,
			      uint32_t id, int error_code);
void gpu_in_dma_user_callback(const struct device *dma_dev, void *arg,
			      uint32_t id, int error_code);
void gpu_out_dma_user_callback(const struct device *dma_dev, void *arg,
			      uint32_t id, int error_code);
void dma_init_cam1();
void dma_init_cam2();
void dma_init_gpu_inputs();
void dma_init_gpu_output();
