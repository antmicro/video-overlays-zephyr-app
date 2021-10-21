#define CSR_BASE 0xf0000000L

unsigned int read_reg(unsigned int offset);
void write_reg(unsigned int to_write, unsigned int offset);
