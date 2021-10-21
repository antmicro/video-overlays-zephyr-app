#include <zephyr.h>
#include <stdio.h>
#include "reg.h"
#include "initiator.h"

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
