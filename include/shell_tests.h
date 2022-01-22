#include "img_gen.h"
#include "gpu.h"
#include "init.h"

extern const struct device* ov2640_dev_1;
extern const struct device* ov2640_dev_2;
extern const struct device* fastvdma_dev_cam_1;
extern const struct device* fastvdma_dev_cam_2;
extern const struct device* fastvdma_dev_gpu_in_1;
extern const struct device* fastvdma_dev_gpu_in_2;
extern const struct device* fastvdma_dev_gpu_out;
extern uint32_t img_buff_1[1280 * 1024];
extern uint32_t img_buff_2[1280 * 1024];
extern uint32_t img_buff_3[1280 * 1024];
extern uint32_t img_buff_4[1280 * 1024];
extern uint32_t img_buff_5[1280 * 1024];
extern uint32_t img_buff_6[1280 * 1024];
extern uint32_t img_buff_7[1280 * 1024];
extern uint32_t* hdmi_buffers[3];
extern uint32_t* hdmi_buffers_overlay[3];
extern uint32_t img_length_1;
extern uint32_t img_length_2;
extern struct video_format fmt_1;
extern struct video_format fmt_2;
extern struct video_caps caps;
extern struct dma_config dma_cfg_cam1;
extern struct dma_config dma_cfg_cam2;
extern struct dma_config dma_cfg_gpu_in;
extern struct dma_config dma_cfg_gpu_out;
extern struct dma_block_config dma_block_cfg_cam;
extern struct dma_block_config dma_block_cfg_gpu_in;
extern struct dma_block_config dma_block_cfg_gpu_out;
extern uint32_t image_with_text[800 * 600];

extern int cam_buffer_index;
extern int gpu_buffer_index;

extern const k_tid_t hdmi_id;
extern const k_tid_t gpu_id;
extern const k_tid_t cam_id;

extern bool suspend_hdmi;
extern bool suspend_cam;
extern bool suspend_gpu;

extern enum mode{cams, overlay} mode;
extern enum mode mode;

extern char* overlay_text;

extern struct k_sem my_sem;

#define IMG2_HEIGHT 800U
#define IMG2_WIDTH 600U
#define IMG2_LINE IMG1_WIDTH * 4
#define IMG2_SIZE IMG1_HEIGHT *IMG1_WIDTH * 4

#define IMG1_HEIGHT 800U
#define IMG1_WIDTH 600U
#define IMG1_LINE IMG1_WIDTH * 4
#define IMG1_SIZE IMG1_HEIGHT *IMG1_WIDTH * 4

#define CHAR_CAN 0x18
#define CHAR_DC1 0x11

void flush_l2_cache(void);
void blend_images(uint32_t read_addr1, uint32_t read_addr2, uint32_t write_addr);
