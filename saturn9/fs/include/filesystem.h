#ifndef FILESYSTEM_H
#define FILESYSTEM_H
#include <stdint.h>


typedef struct RamDisk {
    char current_char;
    unsigned pos;
    char *content;
} RamDisk;

typedef struct File {
    char *path;
    unsigned base;
    unsigned size;
} File;


typedef struct Directory {
    File *files;
    unsigned file_count;
    struct Directory *directories;
} Directory;

void fs_parse_rd(char *source);


#endif