#include <initrd.h>
#include <filesystem.h>
#include <limine.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <mem.h>

struct limine_internal_module *ramdisk;

struct limine_module_response rdresponse;


struct limine_module_request rdrequest = {
    .id=LIMINE_MODULE_REQUEST,
    .revision=0,
    .response = &rdresponse,
    .internal_module_count=1,
    .internal_modules = &ramdisk
};

char read_rd_c(void *address, unsigned offset) {
    char *ptr = (char *)address;
    char rbyte = ptr[offset];
    return rbyte;
}

void parse_rd(void *address, unsigned size) {
    FS_parse_rd(address, size);
}


void initramdisk() {
    for (unsigned _i=0; _i<rdrequest.response->module_count; _i++) {
        void* memaddr = rdrequest.response->modules[_i]->address;
        unsigned memsize = rdrequest.response->modules[_i]->size;
        parse_rd(memaddr, memsize);
    }
}