#include <stdint.h>

#ifndef COMMON_H
#define COMMON_H

void cpuid(uint32_t eax, uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d);
void hcf();
int sys_init_fpu();

#endif
