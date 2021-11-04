#include <zephyr.h>
#include "reg.h"
#include "mmcm.h"

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
