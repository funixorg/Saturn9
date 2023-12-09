#include <initrd.h>
#include <filesystem.h>
#include <limine.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct limine_internal_module *ramdisk;

struct limine_module_response rdresponse;


struct limine_module_request rdrequest = {
    .id=LIMINE_MODULE_REQUEST,
    .revision=0,
    .response = &rdresponse,
    .internal_module_count=1,
    .internal_modules = &ramdisk
};

char *print_uuid(struct limine_uuid uuid) {
    return format("%d-%d-%d-%d", uuid.a, uuid.b, uuid.c, uuid.d);
}


void read_rd(void *address, unsigned size) {
    char buffer[size+1];
    char *ptr = (char *)address;
    for (unsigned _i = 0; _i < size; _i++) {
        buffer[_i] = ptr[_i];
    }
    buffer[size] = '\0';
    printf_serial("CONTENT: %s\n", buffer);
}


void initramdisk() {
    for (unsigned _i=0; _i<rdrequest.response->module_count; _i++) {
        void* memaddr = rdrequest.response->modules[_i]->address;
        unsigned memsize = rdrequest.response->modules[_i]->size;
        read_rd(memaddr, memsize);
        //printf_serial("R: %d\n", byteValue);
    }
    //printf_serial("FILE: %s\n", );
}