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
    while (rdinfo.current_char != '\0' && rdinfo.current_char != '>') {
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

    while (rdinfo.current_char != '\0' && rdinfo.current_char != ']') {
        if (rdinfo.current_char==';') {
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

    while (rdinfo.current_char != '\0' && rdinfo.current_char != '|') {
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

    while (rdinfo.current_char != '\0' && rdinfo.current_char != ')' && rdinfo.current_char != '&') {
        if (rdinfo.current_char == ' ' || rdinfo.current_char == '\n') {}
        else if (rdinfo.current_char == '<') {
            file.path = parse_path();
        } else if (rdinfo.current_char == '[') {
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

    while (rdinfo.current_char != '\0' && rdinfo.current_char != ')') {
        if (rdinfo.current_char == '&') {
            _i++;
            f_advance(); // Skip &
        } else {
            file = parse_file();
            files[_i] = file;
        }
    }

    return files;
}

void process_rd_dir(Directory *directory) {
    if (rdinfo.current_char==' ' || rdinfo.current_char=='\n') {}
    else if (rdinfo.current_char=='<') {
        directory->path = parse_path();
    }
    else if (rdinfo.current_char=='[') {
        directory->base_size=parse_basesize();
    }
    else if (rdinfo.current_char=='|') {
        directory->file_count=parse_filecount();
    }
    else if (rdinfo.current_char=='(') {
        directory->files=parse_files(directory->file_count);
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

    header_size=atoi(buffer, 10);

    free(buffer);
    
    rdinfo.header_size=header_size;
    while (rdinfo.pos<header_size) {
        process_rd_dir(&root_dir);
    }

    return rdinfo.content; // Needed to keep the source in stack
}


char *read_chunk(File *file) {
    unsigned base = file->base_size.base+rdinfo.header_size+3; // Magic index addend
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

Directory *find_dir(char *dirname) {
    if (strcmp(dirname,"/")) {
        return &root_dir;
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
