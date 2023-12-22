#include <mimas.h>
#include <filesystem.h>
#include <initrd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>

unsigned pos=0;
File *image_file;

uint32_t MIM_read_uint32(unsigned int_size) {
    uint8_t buffer[int_size];
    unsigned header_size = FS_get_hsize();
    for (unsigned _i=0; _i<int_size; _i++) {
        uint8_t byte = (uint8_t)read_rd_c(FS_get_rdaddr(), header_size+image_file->base_size.base+pos);
        pos++;
        buffer[_i] = byte;
    }
    return *(uint32_t*)buffer;
}

uint8_t MIM_read_uint8(unsigned int_size) {
    uint8_t buffer[int_size];
    unsigned header_size = FS_get_hsize();
    for (unsigned _i=0; _i<int_size; _i++) {
        uint8_t byte = (uint8_t)read_rd_c(FS_get_rdaddr(), header_size+image_file->base_size.base+pos);
        pos++;
        buffer[_i] = byte;
    }
    return *(uint8_t*)buffer;
}

void MIM_display_mimimg(File *file, unsigned xpos, unsigned ypos, unsigned reducing_scale, unsigned enlarging_scale) {
    pos=0;
    image_file=file;
    uint32_t width = (MIM_read_uint32(8) / reducing_scale) *enlarging_scale;
    uint32_t height = (MIM_read_uint32(8) / reducing_scale) * enlarging_scale;

    unsigned red;
    unsigned green;
    unsigned blue;

    for (unsigned _i=0; _i<height; _i++) {
        for (unsigned _j=0; _j<width; _j++) {
            red = MIM_read_uint8(4);
            green = MIM_read_uint8(4);
            blue = MIM_read_uint8(4);
            GP_draw_pixel(xpos+_j, ypos+_i, rgbtohex(red,green,blue));
        }
    }
}