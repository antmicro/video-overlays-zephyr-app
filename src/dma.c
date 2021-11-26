#include <sys/printk.h>
#include "dma.h"

void dma_deinit1(DMA_typedef *DMA) {
    DMA->CR = 0x0;
}

void dma_init1(DMA_typedef *DMA) {
    DMA->CR = 0x0F;
}

void dma_sanity_check1(DMA_typedef *DMA) {
    printk("CR: %u\n", DMA->CR);
    printk("SR: %u\n", DMA->SR);
    printk("IMR: %u\n", DMA->IMR);
    printk("ISR: %u\n", DMA->ISR);
    printk("RSAR: %u\n", DMA->RSAR);
    printk("RLLR: %u\n", DMA->RLLR);
    printk("RLCR: %u\n", DMA->RLCR);
    printk("RSBLR: %u\n", DMA->RSBLR);
    printk("WSAR: %u\n", DMA->WSAR);
    printk("WLLR: %u\n", DMA->WLLR);
    printk("WLCR: %u\n", DMA->WLCR);
    printk("WSBLR: %u\n", DMA->WSBLR);
}

void dma_config1(DMA_typedef *DMA, DMA_init_typedef *DMA_struct) {
    dma_reader_frontend_config1(DMA, DMA_struct);
    dma_writer_frontend_config1(DMA, DMA_struct);
}

void dma_reader_frontend_config1(DMA_typedef *DMA, DMA_init_typedef *DMA_struct) {
    DMA->RSAR = DMA_struct->read_addr;
    DMA->RLLR = DMA_struct->read_line_length;
    DMA->RLCR = DMA_struct->read_line_count;
    DMA->RSBLR = DMA_struct->read_line_stride;
}

void dma_writer_frontend_config1(DMA_typedef *DMA, DMA_init_typedef *DMA_struct) {
    DMA->WSAR = DMA_struct->write_addr;
    DMA->WLLR = DMA_struct->write_line_length;
    DMA->WLCR = DMA_struct->write_line_count;
    DMA->WSBLR = DMA_struct->write_line_stride;
}

/* END OF FILE ****************************************************************/

