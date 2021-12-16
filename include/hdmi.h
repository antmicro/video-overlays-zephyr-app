#define INITIATOR_ENABLE_REG 		0x3818L
#define INITIATOR_HSCAN_HI 			0x3834L
#define INITIATOR_HSCAN_LOW 		0x3838L
#define INITIATOR_HRES_HI 			0x381cL
#define INITIATOR_HRES_LOW 			0x3820L
#define INITIATOR_HSYNC_START_HI 	0x3824L
#define INITIATOR_HSYNC_START_LOW 	0x3828L
#define INITIATOR_HSYNC_END_HI 		0x382cL
#define INITIATOR_HSYNC_END_LOW 	0x3830L
#define INITIATOR_VRES_HI 			0x383cL
#define INITIATOR_VRES_LOW 			0x3840L
#define INITIATOR_VSYNC_START_HI 	0x3844L
#define INITIATOR_VSYNC_START_LOW 	0x3848L
#define INITIATOR_VSYNC_END_HI 		0x384cL
#define INITIATOR_VSYNC_END_LOW 	0x3850L
#define INITIATOR_VSCAN_HI	 		0x3854L
#define INITIATOR_VSCAN_LOW 		0x3858L
#define INITIATOR_BASE_BYTE_0 		0x385cL
#define INITIATOR_BASE_BYTE_1 		0x3860L
#define INITIATOR_BASE_BYTE_2 		0x3864L
#define INITIATOR_BASE_BYTE_3 		0x3868L
#define INITIATOR_LENGTH_BYTE_0 	0x386cL
#define INITIATOR_LENGTH_BYTE_1 	0x3870L
#define INITIATOR_LENGTH_BYTE_2 	0x3874L
#define INITIATOR_LENGTH_BYTE_3 	0x3878L

#define CLOCKING_MMCM_RESET_REG 	0x388cL
#define CLOCKING_MMCM_REG 			0x3894L
#define CLOCKING_MMCM_DRDY_REG 		0x3898L
#define CLOCKING_MMCM_ADR_REG 		0x389cL
#define CLOCKING_MMCM_DAT_REG_HI 	0x38a0L
#define CLOCKING_MMCM_DAT_REG_LOW 	0x38a4L

#ifndef CSR_BASE
#define CSR_BASE 0xf0000000L
#endif

/* Default structure for HDMI timings */
struct video_timing {
	int pixel_clock;
	int h_active;
	int h_blanking;
	int h_sync_offset;
	int h_sync_width;
	int v_active;
	int v_blanking;
	int v_sync_offset;
	int v_sync_width;
	int established_timing;
	int flags;
};

/* Framebuffer timings configuration */
void fb_get_clock_md(unsigned int pixel_clock, unsigned int *best_m, unsigned int *best_d);
void fb_set_clock(unsigned int pixel_clock);
void timings_write(struct video_timing *vid_t);
void fb_clkgen_write(int m, int d);

/* Initiator configuration */
uint8_t hdmi_out0_core_initiator_enable_read(void);
void hdmi_out0_core_initiator_enable_write(uint8_t v);
void hdmi_out0_core_initiator_hscan_write(uint16_t v);
void hdmi_out0_core_initiator_hres_write(uint16_t v);
void hdmi_out0_core_initiator_hsync_start_write(uint16_t v);
void hdmi_out0_core_initiator_hsync_end_write(uint16_t v);
void hdmi_out0_core_initiator_vres_write(uint16_t v);
void hdmi_out0_core_initiator_vsync_start_write(uint16_t v);
void hdmi_out0_core_initiator_vsync_end_write(uint16_t v);
void hdmi_out0_core_initiator_vscan_write(uint16_t v);
void hdmi_out0_core_initiator_length_write(uint32_t v);
void hdmi_out0_core_initiator_base_write(uint32_t v);

/* MMCM configuration */
uint8_t hdmi_out0_driver_clocking_mmcm_write_read(void);
void hdmi_out0_driver_clocking_mmcm_write_write(int v);
void hdmi_out0_driver_clocking_mmcm_adr_write(uint8_t v);
void hdmi_out0_driver_clocking_mmcm_dat_w_write(uint16_t v);
uint8_t hdmi_out0_driver_clocking_mmcm_drdy_read(void);
void hdmi_out0_driver_clocking_mmcm_reset_write(uint8_t v);
void hdmi_out0_driver_clocking_mmcm_write(int adr, int data);
