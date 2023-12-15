#ifndef FILESYSTEM_H
#define FILESYSTEM_H
#include <stdint.h>


typedef struct RamDisk {
    char current_char;
    unsigned pos;
    unsigned header_size;
    unsigned bin_size;
    void *address;
} RamDisk;

typedef struct BaseSize {
    unsigned base;
    unsigned size;
} BaseSize;

typedef struct File {
    char *path;
    BaseSize base_size;
} File;


typedef struct Directory {
    char *path;
    File *files;
    unsigned file_count;
    unsigned dir_count;
    BaseSize base_size;
    struct Directory *directories;
} Directory;


typedef struct FileList {
    char **paths;
    unsigned file_count;
} FileList;

void fs_parse_rd(void *address, unsigned size);
void parse_dir(Directory *directory);
Directory *find_dir(char *dirname);
char *read_file(char *dirname, char* filename);
FileList *list_dir(char *dirname);
char *read_path(char *full_path);

#endif