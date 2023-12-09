#include <filesystem.h>

RamDisk rdinfo;
Directory root_dir;

unsigned is_parsing=0;

void fs_parse_rd(char *source) {
    rdinfo.pos=0;
    rdinfo.content=source;
    rdinfo.current_char=rdinfo.content[rdinfo.pos];
    is_parsing=1;
    while (is_parsing) {

    }
}