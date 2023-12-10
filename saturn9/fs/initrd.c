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


char *read_rd(void *address, unsigned size) {
    char *buffer=memalloc(size+1);
    char *ptr = (char *)address;
    for (unsigned _i = 0; _i < size; _i++) {
        buffer[_i] = ptr[_i];
    }
    buffer[size] = '\0';
    return buffer;
}


char *parse_rd(void *address, unsigned size) {
    return fs_parse_rd(address, size);
}


void initramdisk() {
    for (unsigned _i=0; _i<rdrequest.response->module_count; _i++) {
        void* memaddr = rdrequest.response->modules[_i]->address;
        unsigned memsize = rdrequest.response->modules[_i]->size;
        parse_rd(memaddr, memsize);
    }
    //char *content = read_file("source2.txt");
    //if (content) { printf_serial("CONTENT: %s\n", content); };
    char *dirname="/";
    FileList *dirlist = list_dir(dirname);
    for (unsigned _i=0; _i<dirlist->file_count; _i++) {
        printf_serial("%s = `%s`\n", dirlist->paths[_i], read_file(dirname, dirlist->paths[_i]));
    }
}