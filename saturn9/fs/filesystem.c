#include <filesystem.h>
#include <mem.h>
#include <stdlib.h>
#include <stdio.h>
#include <initrd.h>


// ACTUAL FS


RamDisk rdinfo;
Directory root_dir;

void FS_advance() {
    rdinfo.pos++;
    rdinfo.current_char=read_rd_c(rdinfo.address, rdinfo.pos);
}

uint32_t FS_read_uint(unsigned int_size) {
    uint8_t bytes[int_size];
    for (unsigned _i=0; _i<int_size; _i++) {
        bytes[_i] = (uint8_t)(rdinfo.current_char);
        FS_advance(); 
    }
    return *(uint32_t*)(bytes);
}

char *FS_read_str(unsigned str_size) {
    char *buffer=memalloc(str_size);
    for (unsigned _i=0; _i<str_size; _i++) {
        buffer[_i] = rdinfo.current_char;
        FS_advance();
    }
    return buffer;
}


File FS_parse_file() {
    File file;

    file.path=FS_read_str(64);
    BaseSize bs = {FS_read_uint(8), FS_read_uint(8)};
    file.base_size = bs;

    return file;
}

void FS_parse_dir(Directory *directory) {
    directory->path=FS_read_str(32);
    directory->base_size = (BaseSize){FS_read_uint(8), FS_read_uint(8)};
    directory->file_count = FS_read_uint(4);
    directory->dir_count = FS_read_uint(4);
    directory->directories = memalloc(sizeof(Directory)*directory->dir_count);
    directory->files = memalloc(sizeof(File)*directory->file_count );

    for (unsigned _fi=0; _fi<directory->file_count ; _fi++) {
        directory->files[_fi]=FS_parse_file();
    }

    for (unsigned _di=0; _di<directory->dir_count; _di++) {
        Directory subdir;FS_parse_dir(&subdir);
        directory->directories[_di]=subdir;
    }
}

void print_dir(Directory dir) {
    printf_serial("DIR: %s\n", dir.path);
    for (unsigned _j=0; _j<dir.file_count; _j++) {
        printf_serial("     FILE: %s\n", dir.files[_j].path);
    }
    for (unsigned _i=0; _i<dir.dir_count; _i++) {
        print_dir(dir.directories[_i]);
    }
}

void FS_parse_rd(void *address, unsigned size) {
    rdinfo.pos=0;
    rdinfo.address=address;
    rdinfo.current_char=read_rd_c(rdinfo.address, rdinfo.pos);
    rdinfo.bin_size=size;
    rdinfo.header_size=FS_read_uint(4)+4;

    FS_parse_dir(&root_dir);
    
    //print_dir(root_dir);
}

void *FS_get_rdaddr() {
    return rdinfo.address;
}
unsigned FS_get_hsize() {
    return rdinfo.header_size;
}




// VFS


char *VFS_read_chunk(File *file) {
    if (!file->base_size.size) { return ""; }
    unsigned base = file->base_size.base+rdinfo.header_size;
    unsigned size = file->base_size.size-1; // Size-1 = index

    char *buffer=memalloc(size);

    for (unsigned _i=0; _i<=size; _i++) {
        buffer[_i]=read_rd_c(rdinfo.address, base+_i);
    }

    return buffer;
}

File *VFS_find_file(Directory *directory, char *filename) {
    for (unsigned _i=0; _i<directory->file_count; _i++) {
        if (strcmp(directory->files[_i].path,filename)) {
            return &(directory->files[_i]);
        }
    }
    return NULL;
}

Directory *VFS_iterate_dir(Directory *directory, char *dirname) {
    for (unsigned _i=0; _i<directory->dir_count; _i++) {
        if (strcmp(directory->directories[_i].path,dirname)) {
            return &(directory->directories[_i]);
        }
    }
    return NULL;
}


Directory *VFS_find_dir(char *dirname) {
    if (strcmp(dirname,"/")) {
        return &root_dir;
    }
    else {
        if (dirname[0]!='/') { return NULL; }
        char **tokens = tok_split(dirname, '/');
        Directory *current_dir=&root_dir;
        for (unsigned _i=1; tokens[_i]; _i++) {
            if (strcmp(tokens[_i], "")) { continue;}
            current_dir=VFS_iterate_dir(current_dir, tokens[_i]);
            if (!current_dir) { 
                return NULL;
            }
        }
        return current_dir;
    }
    return NULL;
}

char *VFS_read_file(char *dirname, char* filename) {
    Directory *directory = VFS_find_dir(dirname);
    if (directory) {
        File *file = VFS_find_file(directory, filename);
        if (file) {
            char *chunk = VFS_read_chunk(file);
            return chunk;
        }
        return NULL;

    } 
    return NULL;
}

FileList *VFS_list_dir(char *dirname) {
    Directory *directory = VFS_find_dir(dirname);
    if (directory) {
        char **files = memalloc((directory->file_count) * sizeof(char *));
        char **dirs = memalloc((directory->dir_count) * sizeof(char *));
        FileList *file_list = memalloc(sizeof(FileList));
        if (files == NULL) {
            return file_list;
        }
        for (unsigned _i=0; _i < directory->file_count; _i++) {
            files[_i] = directory->files[_i].path;
        }

        for (unsigned _j = 0; _j < directory->dir_count; _j++) {
            dirs[_j] = directory->directories[_j].path;
        }
        if (file_list == NULL) {
            return NULL;
        }
        file_list->paths = files;
        file_list->dirs = dirs;
        file_list->file_count = directory->file_count;
        file_list->dir_count=directory->dir_count;

        return file_list;
    }

    return NULL;
}

File *VFS_get_file(char *full_path) {
    unsigned slashcount=0;
    
    for (unsigned _i=0; full_path[_i]!='\0'; _i++) {
        if (full_path[_i]=='/') {
            slashcount++;
        }
    }

    char *path=memalloc(strlen(full_path));
    for (unsigned _i=1; _i<=strlen(full_path); _i++) {
        path[_i-1] = full_path[_i];
    }

    char **tokens = tok_split(path, '/');
    char *file=memalloc(strlen(full_path));
    Directory *current_dir=&root_dir;
    for (unsigned _i=0; tokens[_i]; _i++) {
        if (_i+1>=slashcount) { 
            file=tokens[_i];
            break;
        }
        if (strcmp(tokens[_i], "")) { continue; }
        current_dir=VFS_iterate_dir(current_dir, tokens[_i]);
        if (!current_dir) { 
            return NULL;
        }
    }

    File *file_handle = VFS_find_file(current_dir, file);
    if (file_handle) { return file_handle; }
    return NULL;
}

char *VFS_read_path(char *full_path) {
    unsigned slashcount=0;
    
    for (unsigned _i=0; full_path[_i]!='\0'; _i++) {
        if (full_path[_i]=='/') {
            slashcount++;
        }
    }

    char *path=memalloc(strlen(full_path));
    for (unsigned _i=1; _i<=strlen(full_path); _i++) {
        path[_i-1] = full_path[_i];
    }

    char **tokens = tok_split(path, '/');
    char *file=memalloc(strlen(full_path));
    Directory *current_dir=&root_dir;
    for (unsigned _i=0; tokens[_i]; _i++) {
        if (_i+1>=slashcount) { 
            file=tokens[_i];
            break;
        }
        if (strcmp(tokens[_i], "")) { continue; }
        current_dir=VFS_iterate_dir(current_dir, tokens[_i]);
        if (!current_dir) { 
            return NULL;
        }
    }

    File *file_handle = VFS_find_file(current_dir, file);
    if (file_handle) { return VFS_read_chunk(file_handle); }
    return NULL;
}