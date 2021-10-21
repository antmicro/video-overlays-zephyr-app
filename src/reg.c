#include "reg.h"

unsigned int read_reg(unsigned int offset) {
	  volatile unsigned int *reg = (unsigned int*)(CSR_BASE + offset);
	  return *reg;
}

void write_reg(unsigned int to_write, unsigned int offset){
	volatile unsigned int *reg = (unsigned int*)(CSR_BASE + offset);
	*reg = to_write;
}
