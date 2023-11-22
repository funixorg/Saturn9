#include <stdint.h>
#include <pmm.h>
#include <limine.h>


struct limine_stack_size_response stack_size_response;

struct limine_stack_size_request stack_size_request = {
    .id = LIMINE_STACK_SIZE_REQUEST,
    .revision =0,
    .response = &stack_size_response,
    STACK_SIZE
};

struct limine_paging_mode_response paging_response;

struct limine_paging_mode_request paging_request = {
    .id = LIMINE_PAGING_MODE_REQUEST,
    .revision = 0,
    .response = &paging_response,
    .mode = LIMINE_PAGING_MODE_X86_64_4LVL,
    .flags = 0,
};

struct limine_smp_response smp_response;

struct limine_smp_request smp_request = {
    .id = LIMINE_SMP_REQUEST,
    .revision = 0,
    .response = &smp_response,
    .flags = 0
};

struct limine_memmap_response memmap_response;

struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0,
    .response = &memmap_response
};

struct limine_boot_time_response boottime_response;

struct limine_boot_time_request boottime_request = {
    .id = LIMINE_BOOT_TIME_REQUEST,
    .revision = 0,
    .response = &boottime_response
};

struct limine_kernel_address_response kernel_addr_response;

struct limine_kernel_address_request kernel_addr_request = {
    .id = LIMINE_KERNEL_ADDRESS_REQUEST,
    .revision = 0,
    .response = &kernel_addr_response
};

long long get_avaiable_mem() {
    uint64_t memsize=0;
    for (unsigned i=0; i<8; i++) {
        memsize+=memmap_request.response->entries[i]->length;
    }
    return memsize;
}


uint64_t get_cpu_count() {
    return smp_request.response->cpu_count;
}

int64_t get_boot_time() {
    return boottime_request.response->boot_time;
}

uint64_t get_kernel_addr() {
    return kernel_addr_request.response->physical_base;
}