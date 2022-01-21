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
extern uint32_t image_with_text[1280 * 1024];
extern uint32_t img_buff_1[1280 * 1024];
extern uint32_t img_buff_2[1280 * 1024];
extern uint32_t img_buff_3[1280 * 1024];
extern uint32_t img_buff_7[1280 * 1024];
extern uint32_t img_buff_8[1280 * 1024];
extern uint32_t img_buff_9[1280 * 1024];
extern uint32_t img_buff_10[1280 * 1024];
extern uint32_t* hdmi_buffers1[3];
extern uint32_t* hdmi_buffers3[3];
extern int cam_buffer_index;
extern int gpu_buffer_index;

extern int block_buff[3];
extern int blocked_buff_gpu;
extern int blocked_buff_cam1;
extern bool callback_cam_block;
extern bool callback_gpu_block;

extern const k_tid_t cam_id;
extern const k_tid_t gpu_id;
extern const k_tid_t hdmi_id;

extern enum mode mode;
extern struct k_sem my_sem;

void cam1_dma_user_callback(const struct device *dma_dev, void *arg,
			      uint32_t id, int error_code);
void cam1_with_gpu_dma_user_callback(const struct device *dma_dev, void *arg,
			      uint32_t id, int error_code);
void cam2_dma_user_callback(const struct device *dma_dev, void *arg,
			      uint32_t id, int error_code);
void cam2_with_gpu_dma_user_callback(const struct device *dma_dev, void *arg,
			      uint32_t id, int error_code);
void gpu_in_dma_user_callback(const struct device *dma_dev, void *arg,
			      uint32_t id, int error_code);
void gpu_out_dma_user_callback(const struct device *dma_dev, void *arg,
			      uint32_t id, int error_code);
void dma_init_cam1();
void dma_init_cam2();
void dma_init_gpu_inputs();
void dma_init_gpu_output();
void hdmi(void);
void cam(void);
void gpu(void);
void suspend_threads();