#include <filesystem.h>
#include <mem.h>
#include <stdlib.h>
#include <stdio.h>
#include <initrd.h>


// ACTUAL FS


RamDisk rdinfo;
Directory root_dir;

void f_advance() {
    rdinfo.pos++;
    rdinfo.current_char=read_rd_c(rdinfo.address, rdinfo.pos);
}

uint32_t read_uint(unsigned int_size) {
    uint8_t bytes[int_size];
    for (unsigned _i=0; _i<int_size; _i++) {
        bytes[_i] = (uint8_t)(rdinfo.current_char);
        f_advance(); 
    }
    return *(uint32_t*)(bytes);
}

char *read_str(unsigned str_size) {
    char *buffer=memalloc(str_size);
    for (unsigned _i=0; _i<str_size; _i++) {
        buffer[_i] = rdinfo.current_char;
        f_advance();
    }
    return buffer;
}


File parse_file() {
    File file;

    file.path=read_str(64);
    BaseSize bs = {read_uint(8), read_uint(8)};
    file.base_size = bs;

    return file;
}

void parse_dir(Directory *directory) {
    directory->path=read_str(32);
    directory->base_size = (BaseSize){read_uint(8), read_uint(8)};
    directory->file_count = read_uint(4);
    directory->dir_count = read_uint(4);
    directory->directories = memalloc(sizeof(Directory)*directory->dir_count);
    directory->files = memalloc(sizeof(File)*directory->file_count );

    for (unsigned _fi=0; _fi<directory->file_count ; _fi++) {
        directory->files[_fi]=parse_file();
    }

    for (unsigned _di=0; _di<directory->dir_count; _di++) {
        Directory subdir;parse_dir(&subdir);
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

void fs_parse_rd(void *address, unsigned size) {
    rdinfo.pos=0;
    rdinfo.address=address;
    rdinfo.current_char=read_rd_c(rdinfo.address, rdinfo.pos);
    rdinfo.bin_size=size;
    rdinfo.header_size=read_uint(4)+4;

    parse_dir(&root_dir);
    
    //print_dir(root_dir);
}





// VFS


char *read_chunk(File *file) {
    if (!file->base_size.size) { return ""; }
    unsigned base = file->base_size.base+rdinfo.header_size;
    unsigned size = file->base_size.size-1; // Size-1 = index

    char *buffer=memalloc(size);

    for (unsigned _i=0; _i<=size; _i++) {
        buffer[_i]=read_rd_c(rdinfo.address, base+_i);
    }

    return buffer;
}

File *find_file(Directory *directory, char *filename) {
    for (unsigned _i=0; _i<directory->file_count; _i++) {
        if (strcmp(directory->files[_i].path,filename)) {
            return &(directory->files[_i]);
        }
    }
    return NULL;
}

Directory *iterate_dir(Directory *directory, char *dirname) {
    for (unsigned _i=0; _i<directory->dir_count; _i++) {
        if (strcmp(directory->directories[_i].path,dirname)) {
            return &(directory->directories[_i]);
        }
    }
    return NULL;
}


Directory *find_dir(char *dirname) {
    if (strcmp(dirname,"/")) {
        return &root_dir;
    }
    else {
        if (dirname[0]!='/') { return NULL; }
        char **tokens = strsplit(dirname, '/');
        Directory *current_dir=&root_dir;
        for (unsigned _i=1; tokens[_i]; _i++) {
            if (strcmp(tokens[_i], "")) { continue; }
            current_dir=iterate_dir(current_dir, tokens[_i]);
            if (!current_dir) { 
                return NULL;
            }
        }
        return current_dir;
    }
    return NULL;
}

char *read_file(char *dirname, char* filename) {
    Directory *directory = find_dir(dirname);
    if (directory) {
        File *file = find_file(directory, filename);
        if (file) {
            char *chunk = read_chunk(file);
            return chunk;
        }
        return NULL;

    } 
    return NULL;
}

FileList *list_dir(char *dirname) {
    Directory *directory = find_dir(dirname);
    if (directory) {
        unsigned count = directory->file_count;
        char **files = memalloc(count * sizeof(char *));
        if (files == NULL) {
            return NULL;
        }

        for (unsigned _i = 0; _i < count; _i++) {
            files[_i] = directory->files[_i].path;
        }

        FileList *file_list = memalloc(sizeof(FileList));
        if (file_list == NULL) {
            free(files);
            return NULL;
        }

        file_list->paths = files;
        file_list->file_count = count;

        return file_list;
    }

    return NULL;
}

char *read_path(char *full_path) {
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
        current_dir=iterate_dir(current_dir, tokens[_i]);
        if (!current_dir) { 
            return NULL;
        }
    }

    File *file_handle = find_file(current_dir, file);
    if (file_handle) { return read_chunk(file_handle); }
    return NULL;
}