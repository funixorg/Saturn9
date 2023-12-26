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
    char **dirs;
    unsigned file_count;
    unsigned dir_count;
} FileList;

void FS_parse_rd(void *address, unsigned size);
void FS_parse_dir(Directory *directory);
Directory *VFS_find_dir(char *dirname);
char *VFS_read_file(char *dirname, char* filename);
FileList *VFS_list_dir(char *dirname);
char *VFS_read_path(char *full_path);
File *VFS_get_file(char *full_path);
void *FS_get_rdaddr();
unsigned FS_get_hsize();

#endif