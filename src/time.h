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

void fb_get_clock_md(unsigned int pixel_clock, unsigned int *best_m, unsigned int *best_d);
void fb_set_clock(unsigned int pixel_clock);
void timings_write(struct video_timing *vid_t);
void fb_clkgen_write(int m, int d);
