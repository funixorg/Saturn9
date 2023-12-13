#include <filesystem.h>
#include <mem.h>
#include <stdlib.h>
#include <stdio.h>
#include <initrd.h>

RamDisk rdinfo;
Directory root_dir;

void f_advance() {
    rdinfo.pos++;
    rdinfo.current_char=rdinfo.content[rdinfo.pos];
}

char *parse_path() {
    char *buffer=memalloc(128);
    unsigned _i=0;
    f_advance(); // Skip <
    while (rdinfo.current_char != '\0' && rdinfo.current_char != 0x2) {
        buffer[_i]=rdinfo.current_char;
        f_advance();
        _i++;
    }
    buffer[_i]='\0';

    return buffer;
}

BaseSize parse_basesize() {
    char *buffer=memalloc(16);
    unsigned base=0;
    unsigned size=0;

    unsigned _i=0;
    f_advance(); // Skip [

    while (rdinfo.current_char != '\0' && rdinfo.current_char != 0x6) {
        if (rdinfo.current_char==0x1f) {
            buffer[_i+1]='\0';
            base=atoi(buffer,10);
            _i=0;
            memset(buffer,0,128);
        }
        else {
            buffer[_i]=rdinfo.current_char;
            _i++;
        }

        f_advance();
    }
    buffer[_i]='\0';
    size=atoi(buffer,10);

    free(buffer);

    BaseSize basesize={base,size};
    return basesize;
}

unsigned parse_filecount() {
    char *buffer=memalloc(16);
    unsigned count=0;
    
    unsigned _i=0;
    f_advance(); // Skip |

    while (rdinfo.current_char != '\0' && rdinfo.current_char != 0x3) {
        buffer[_i]=rdinfo.current_char;
        _i++;
        f_advance();
    }
    buffer[_i]='\0';

    count=atoi(buffer,10);

    free(buffer);

    return count;
}

unsigned parse_dircount() {
    char *buffer=memalloc(16);
    unsigned count=0;
    
    unsigned _i=0;
    f_advance(); // Skip #

    while (rdinfo.current_char != '\0' && rdinfo.current_char != 0x4) {
        buffer[_i]=rdinfo.current_char;
        _i++;
        f_advance();
    }
    buffer[_i]='\0';

    count=atoi(buffer,10);

    free(buffer);

    return count;
}

File parse_file() {
    File file;

    while (rdinfo.current_char != '\0' && rdinfo.current_char != 0x18 && rdinfo.current_char != 0x11) {
        if (rdinfo.current_char == ' ' || rdinfo.current_char == '\n') {}
        else if (rdinfo.current_char == 0x1) {
            file.path = parse_path();
        } else if (rdinfo.current_char == 0x5) {
            file.base_size = parse_basesize();
        }
        f_advance();
    }
    
    return file;
}

File *parse_files(unsigned file_count) {
    File *files=memalloc(file_count * sizeof(File));
    File file;
    unsigned _i = 0;

    f_advance(); // Skip (

    while (rdinfo.current_char != '\0' && rdinfo.current_char != 0x18) {
        if (rdinfo.current_char == 0x11) {
            _i++;
            f_advance(); // Skip &
        } else {
            file = parse_file();
            files[_i] = file;
        }
    }

    return files;
}

Directory *parse_dirs(unsigned dirs_count) {
    Directory *dirs=memalloc(dirs_count * sizeof(Directory));
    Directory dir;
    unsigned _i = 0;

    f_advance(); // Skip (

    while (rdinfo.current_char != '\0' && rdinfo.current_char != 0x15) {
        if (rdinfo.current_char == 0x12) {
            _i++;
            f_advance(); // Skip &
        } else {
            parse_dir(&dir);
            dirs[_i] = dir;
        }
    }

    return dirs;
}

void parse_dir(Directory *directory) {
    if (rdinfo.current_char==' ' || rdinfo.current_char=='\n') {}
    else if (rdinfo.current_char==0x1) {
        directory->path = parse_path();
    }
    else if (rdinfo.current_char==0x5) {
        directory->base_size=parse_basesize();
    }
    else if (rdinfo.current_char==0x3) {
        directory->file_count=parse_filecount();
    }
    else if (rdinfo.current_char==0x4) {
        directory->dir_count=parse_dircount();
    }
    else if (rdinfo.current_char==0x17) {
        directory->files=parse_files(directory->file_count);
    }
    else if (rdinfo.current_char==0x19) {
        directory->directories=parse_dirs(directory->dir_count);
    }
    f_advance();
}


char *fs_parse_rd(void *address, unsigned size) {
    rdinfo.pos=0;
    rdinfo.content=read_rd(address,size);
    rdinfo.current_char=rdinfo.content[rdinfo.pos];
    rdinfo.bin_size=size;
    rdinfo.address=address;

    unsigned header_size=0;
    char *buffer=memalloc(16);

    unsigned _i=0;
    for (;rdinfo.current_char!='\n'; _i++) {
        buffer[_i]=rdinfo.current_char;
        f_advance();
    }
    buffer[_i+1]='\0';

    header_size=atoi(buffer, 10)+strlen(buffer)+1; // Header Size + HeaderSize_int length + newline

    free(buffer);
    
    rdinfo.header_size=header_size;
    f_advance(); // Skip newline
    while (rdinfo.pos<header_size) {
        parse_dir(&root_dir);
    }

    return rdinfo.content;
}


char *read_chunk(File *file) {
    if (!file->base_size.size) { return ""; }
    unsigned base = file->base_size.base+rdinfo.header_size;
    unsigned size = file->base_size.size-1; // Size-1 = index

    char *buffer=memalloc(size);

    for (unsigned _i=0; _i<=size; _i++) {
        buffer[_i]=rdinfo.content[base+_i];
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
    unsigned slashcount=1;
    
    for (unsigned _i=0; full_path[_i]!='\0'; _i++) {
        if (full_path[_i]=='/') {
            slashcount++;
        }
    }

    char **tokens = strsplit(full_path, '/');
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
            printf_serial("HERE %s\n", tokens[_i]);
            return NULL;
        }
    }

    File *file_handle = find_file(current_dir, file);
    if (file_handle) { return read_chunk(file_handle); }
    return NULL;
}