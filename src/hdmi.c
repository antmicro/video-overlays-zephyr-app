#include <zephyr.h>
#include <stdio.h>
#include <stdlib.h>

#include "hdmi.h"

/* Common read/write functions */
unsigned int read_reg(unsigned int offset) {
	volatile unsigned int *reg = (unsigned int*)(CSR_BASE + offset);
	return *reg;
}

void write_reg(unsigned int to_write, unsigned int offset){
	volatile unsigned int *reg = (unsigned int*)(CSR_BASE + offset);
	*reg = to_write;
}

/* Framebuffer timings configuration */
void fb_get_clock_md(unsigned int pixel_clock, unsigned int *best_m, unsigned int *best_d)
{
	unsigned int max_m, max_d;
	unsigned int ideal_m, ideal_d;
	unsigned int bm, bd;
	unsigned int m, d;
	unsigned int diff_current;
	unsigned int diff_tested;
	ideal_m = pixel_clock;
	bm = 1;
	bd = 0;
	pixel_clock = pixel_clock * 10;
	ideal_d = 1000;
	max_d = 128;
	max_m = 128;
	for(d=1;d<=max_d;d++)
		for(m=2;m<=max_m;m++) {
			/* common denominator is d*bd*ideal_d */
			diff_current = abs(d*ideal_d*bm - d*bd*ideal_m);
			diff_tested = abs(bd*ideal_d*m - d*bd*ideal_m);
			if(diff_tested < diff_current) {
				bm = m;
				bd = d;
			}
		}
	*best_m = bm;
	*best_d = bd;
}

void fb_set_clock(unsigned int pixel_clock)
{
	unsigned int clock_m, clock_d;
	fb_get_clock_md(pixel_clock, &clock_m, &clock_d);
	fb_clkgen_write(clock_m, clock_d);
}

void timings_write(struct video_timing *vid_t){
	unsigned int hdmi_out0_enabled;
	if (hdmi_out0_core_initiator_enable_read()) {
		hdmi_out0_enabled = 1;
		hdmi_out0_core_initiator_enable_write(0);
	}
	hdmi_out0_core_initiator_hres_write(vid_t->h_active);
	hdmi_out0_core_initiator_hsync_start_write(vid_t->h_active + vid_t->h_sync_offset);
	hdmi_out0_core_initiator_hsync_end_write(vid_t->h_active + vid_t->h_sync_offset + vid_t->h_sync_width);
	hdmi_out0_core_initiator_hscan_write(vid_t->h_active + vid_t->h_blanking);
	hdmi_out0_core_initiator_vres_write(vid_t->v_active);
	hdmi_out0_core_initiator_vsync_start_write(vid_t->v_active + vid_t->v_sync_offset);
	hdmi_out0_core_initiator_vsync_end_write(vid_t->v_active + vid_t->v_sync_offset + vid_t->v_sync_width);
	hdmi_out0_core_initiator_vscan_write(vid_t->v_active + vid_t->v_blanking);
	hdmi_out0_core_initiator_length_write(vid_t->h_active*vid_t->v_active*4);
	hdmi_out0_core_initiator_enable_write(hdmi_out0_enabled);
	fb_set_clock(vid_t->pixel_clock);
}

void fb_clkgen_write(int m, int d)
{
	/* MMCM VCO range for slowest speed grade Artix-7 is 600MHz-1200MHz
	   Warn if the VCO strays outside these limits.*/
	if ((((m*100)/d) > 1200) || (((m*100)/d) < 600)) {
		printk(
			"WARNING: VCO freq (%uMHz) outside 600MHz-1200MHz range\n"
			"          M: %d, D: %d\n",
			(m*100)/d, m, d);
	} else {
		printk(
			"INFO: VCO freq: %uMHz, M: %d, D: %d\n", (m*100)/d, m, d);
	}
	/* clkfbout_mult = m */

	if(m%2){
		hdmi_out0_driver_clocking_mmcm_write(0x14, 0x1000 | ((m/2)<<6) | (m/2 + 1));
	}
	else{
		hdmi_out0_driver_clocking_mmcm_write(0x14, 0x1000 | ((m/2)<<6) | m/2);
	}
	/* divclk_divide = d */
	if (d == 1)
		hdmi_out0_driver_clocking_mmcm_write(0x16, 0x1000);
	else if(d%2)
		hdmi_out0_driver_clocking_mmcm_write(0x16, ((d/2)<<6) | (d/2 + 1));
	else
		hdmi_out0_driver_clocking_mmcm_write(0x16, ((d/2)<<6) | d/2);
	/* clkout0_divide = 10 */
	hdmi_out0_driver_clocking_mmcm_write(0x8, 0x1000 | (5<<6) | 5);
	/* clkout1_driver_clocking_divide = 2; */
	hdmi_out0_driver_clocking_mmcm_write(0xa, 0x1000 | (1<<6) | 1);
}


/* Initiator configuration */
uint8_t hdmi_out0_core_initiator_enable_read(void){
	return read_reg(INITIATOR_ENABLE_REG);
}
void hdmi_out0_core_initiator_enable_write(uint8_t v){
	write_reg(v, INITIATOR_ENABLE_REG);
}
void hdmi_out0_core_initiator_hscan_write(uint16_t v){
	write_reg(v >> 8, INITIATOR_HSCAN_HI);
	write_reg(v, INITIATOR_HSCAN_LOW);
}
void hdmi_out0_core_initiator_hres_write(uint16_t v){
	write_reg(v >> 8, INITIATOR_HRES_HI);
	write_reg(v, INITIATOR_HRES_LOW);
}

void hdmi_out0_core_initiator_hsync_start_write(uint16_t v){
	write_reg(v >> 8, INITIATOR_HSYNC_START_HI);
	write_reg(v, INITIATOR_HSYNC_START_LOW);
}
void hdmi_out0_core_initiator_hsync_end_write(uint16_t v){
	write_reg(v >> 8, INITIATOR_HSYNC_END_HI);
	write_reg(v, INITIATOR_HSYNC_END_LOW);
}
void hdmi_out0_core_initiator_vres_write(uint16_t v){
	write_reg(v >> 8, INITIATOR_VRES_HI);
	write_reg(v, INITIATOR_VRES_LOW);
}

void hdmi_out0_core_initiator_vsync_start_write(uint16_t v){
	write_reg(v >> 8, INITIATOR_VSYNC_START_HI);
	write_reg(v, INITIATOR_VSYNC_START_LOW);
}

void hdmi_out0_core_initiator_vsync_end_write(uint16_t v){
	write_reg(v >> 8, INITIATOR_VSYNC_END_HI);
	write_reg(v, INITIATOR_VSYNC_END_LOW);
}

void hdmi_out0_core_initiator_vscan_write(uint16_t v){
	write_reg(v >> 8, INITIATOR_VSCAN_HI);
	write_reg(v, INITIATOR_VSCAN_LOW);
}

void hdmi_out0_core_initiator_length_write(uint32_t v){
	write_reg(v >> 24, INITIATOR_LENGTH_BYTE_0);
	write_reg(v >> 16, INITIATOR_LENGTH_BYTE_1);
	write_reg(v >> 8, INITIATOR_LENGTH_BYTE_2);
	write_reg(v, INITIATOR_LENGTH_BYTE_3);
}

void hdmi_out0_core_initiator_base_write(uint32_t v){
	write_reg(v >> 24, INITIATOR_BASE_BYTE_0);
	write_reg(v >> 16, INITIATOR_BASE_BYTE_1);
	write_reg(v >> 8, INITIATOR_BASE_BYTE_2);
	write_reg(v, INITIATOR_BASE_BYTE_3);
}

/* MMCM configuration */
uint8_t hdmi_out0_driver_clocking_mmcm_write_read(void){
	return read_reg(CLOCKING_MMCM_REG);
}
void hdmi_out0_driver_clocking_mmcm_write_write(int v){
	write_reg(v, CLOCKING_MMCM_REG);
}

void hdmi_out0_driver_clocking_mmcm_adr_write(uint8_t v){
	write_reg(v, CLOCKING_MMCM_ADR_REG);
}

void hdmi_out0_driver_clocking_mmcm_dat_w_write(uint16_t v){
	write_reg(v >> 8, CLOCKING_MMCM_DAT_REG_HI);
	write_reg(v, CLOCKING_MMCM_DAT_REG_LOW);
}

uint8_t hdmi_out0_driver_clocking_mmcm_drdy_read(void){
	return read_reg(CLOCKING_MMCM_DRDY_REG);
}

void hdmi_out0_driver_clocking_mmcm_reset_write(uint8_t v){
	write_reg(v, CLOCKING_MMCM_RESET_REG);
}

void hdmi_out0_driver_clocking_mmcm_write(int adr, int data)
{
	hdmi_out0_driver_clocking_mmcm_adr_write(adr);
	hdmi_out0_driver_clocking_mmcm_dat_w_write(data);
	hdmi_out0_driver_clocking_mmcm_write_write(1);
	while(!hdmi_out0_driver_clocking_mmcm_drdy_read());
}
