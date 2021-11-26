#ifndef __LITEX_VEXRISCV_H
#define __LITEX_VEXRISCV_H

#ifdef __cplusplus
    extern "C" {
#endif /* __cplusplus */

/** @brief IP cores registers
  * @{
  */

/* FastVDMA */
typedef struct {
    volatile uint32_t CR;
    volatile uint32_t SR;
    volatile uint32_t IMR;
    volatile uint32_t ISR;
    volatile uint32_t RSAR;
    volatile uint32_t RLLR;
    volatile uint32_t RLCR;
    volatile uint32_t RSBLR;
    volatile uint32_t WSAR;
    volatile uint32_t WLLR;
    volatile uint32_t WLCR;
    volatile uint32_t WSBLR;
} DMA_typedef;

/* GPU */
typedef struct {
     uint8_t CR;          /* offset 0x00*/
     uint8_t reserved1;   /* offset 0x01*/
     uint8_t reserved2;   /* offset 0x02*/
     uint8_t reserved3;   /* offset 0x03*/
     uint8_t FILL_CLR;    /* offset 0x04*/
     uint8_t reserved4;   /* offset 0x05*/
     uint8_t reserved5;   /* offset 0x06*/
     uint8_t reserved6;   /* offset 0x07*/
     uint8_t FILL_CLR_2;  /* offset 0x08*/
     uint8_t reserved7;   /* offset 0x09*/
     uint8_t reserved8;   /* offset 0x0A*/
     uint8_t reserved9;   /* offset 0x0B*/
     uint8_t FILL_CLR_3;  /* offset 0x0C*/
     uint8_t reserved10;  /* offset 0x0D*/
     uint8_t reserved11;  /* offset 0x0E*/
     uint8_t reserved12;  /* offset 0x0F*/
     uint8_t FILL_CLR_4;  /* offset 0x10*/
} GPU_typedef;

/**
  * @}
  */

/** @brief IP cores base addresses
  * @{
  */

#define CSR_BASE        (0xf0000000L)

/* FastVDMA */
#define DMA1_BASE       (0x8000c000UL)
#define DMA1            ((volatile DMA_typedef*) DMA1_BASE)
#define DMA2_BASE       (0x8000d000UL)
#define DMA2            ((volatile DMA_typedef*) DMA2_BASE)
#define DMA3_BASE       (0x8000e000UL)
#define DMA3            ((volatile DMA_typedef*) DMA3_BASE)

/* GPU */
#define GPU_BASE        (CSR_BASE + 0x4000L)
#define GPU             ((volatile GPU_typedef*) GPU_BASE)

/**
  * @}
  */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __LITEX_VEXRISCV_H */
