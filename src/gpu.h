#ifndef __GPU_H
#define __GPU_H

#ifdef __cplusplus
    extern "C" {
#endif /* __cplusplus */

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

