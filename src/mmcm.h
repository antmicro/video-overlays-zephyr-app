#define CLOCKING_MMCM_RESET_REG 0x388c
#define CLOCKING_MMCM_REG 0x3894
#define CLOCKING_MMCM_DRDY_REG 0x3898
#define CLOCKING_MMCM_ADR_REG 0x389c
#define CLOCKING_MMCM_DAT_REG_HI 0x38a0
#define CLOCKING_MMCM_DAT_REG_LOW 0x38a4

uint8_t hdmi_out0_driver_clocking_mmcm_write_read(void);
void hdmi_out0_driver_clocking_mmcm_write_write(int v);
void hdmi_out0_driver_clocking_mmcm_adr_write(uint8_t v);
void hdmi_out0_driver_clocking_mmcm_dat_w_write(uint16_t v);
uint8_t hdmi_out0_driver_clocking_mmcm_drdy_read(void);
void hdmi_out0_driver_clocking_mmcm_reset_write(uint8_t v);
void hdmi_out0_driver_clocking_mmcm_write(int adr, int data);
