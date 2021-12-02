#ifndef __GPU_H
#define __GPU_H

#ifdef __cplusplus
    extern "C" {
#endif /* __cplusplus */

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

#define CSR_BASE        (0xf0000000L)

/* GPU */
#define GPU_BASE        (CSR_BASE + 0x4000L)
#define GPU             ((volatile GPU_typedef*) GPU_BASE)

/* GPU operating modes */
#define GPU_CR_IDLE             ((uint32_t)0x00000000)
#define GPU_CR_ALPHA_BLENDER    ((uint32_t)0x00000001)
#define GPU_CR_FILL_AREA        ((uint32_t)0x00000002)

/* Colors */
#define CLR_RED                 ((uint32_t)0xFF0000FF)
#define CLR_GREEN               ((uint32_t)0xFF00FF00)
#define CLR_BLUE                ((uint32_t)0xFFFF0000)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /* __GPU_H */

/* END OF FILE ****************************************************************/

