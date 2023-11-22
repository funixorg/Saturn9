#include <stdint.h>

#ifndef PMM_H
#define PMM_H

#define STACK_SIZE 8192
#define LIMINE_PAGING_MODE_X86_64_4LVL 0

long long get_avaiable_mem();
uint64_t get_cpu_count();
int64_t get_boot_time();
uint64_t get_kernel_addr();

#endif