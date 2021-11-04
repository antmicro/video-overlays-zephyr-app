#include <zephyr.h>
#include <stdlib.h>
#include "initiator.h"
#include "mmcm.h"
#include "time.h"

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
	hdmi_out0_core_initiator_length_write(vid_t->h_active*vid_t->v_active*2);
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

