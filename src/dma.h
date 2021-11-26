#ifndef __DMA_H
#define __DMA_H

#ifdef __cplusplus
    extern "C" {
#endif /* __cplusplus */

#include <zephyr.h>
#include "litex_vexriscv.h"

/**
  * @brief DMA init structure definition
  */
typedef struct {
    uint32_t read_addr;
    uint32_t read_line_length;
    uint32_t read_line_count;
    uint32_t read_line_stride;
    uint32_t write_addr;
    uint32_t write_line_length;
    uint32_t write_line_count;
    uint32_t write_line_stride;
} DMA_init_typedef;

/* DMA initialization and configuration functions ----------------------------*/
void dma_deinit1(DMA_typedef *DMA);
void dma_init1(DMA_typedef *DMA);
void dma_sanity_check1(DMA_typedef *DMA);

void dma_config1(DMA_typedef *DMA, DMA_init_typedef *DMA_struct);
void dma_reader_frontend_config1(DMA_typedef *DMA, DMA_init_typedef *DMA_struct);
void dma_writer_frontend_config1(DMA_typedef *DMA, DMA_init_typedef *DMA_struct);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __DMA_H */

/* END OF FILE ****************************************************************/

