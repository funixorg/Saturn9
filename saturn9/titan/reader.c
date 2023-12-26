#include <titan_reader.h>
#include <mem.h>
#include <filesystem.h>
#include <initrd.h>
#include <stdint.h>
#include <datatypes.h>

// ----- Defining here as i won't create a datatypes.c file

DataType TYPE_INT32 = {sizeof(uint32_t), 0};
DataType TYPE_INT8 = {sizeof(uint8_t), 1};
DataType TYPE_POINTER = {sizeof(uint64_t), 2};
DataType TYPE_REGISTER = {sizeof(short), 3};
DataType TYPE_IDENTIFIER = {sizeof(char*), 4};

// -----

unsigned titan_pos=0;
File *binary_file;

uint32_t TITAN_read_uint32(unsigned int_size) {
    uint8_t buffer[int_size];
    unsigned header_size = FS_get_hsize();
    for (unsigned _i = 0; _i < int_size; _i++) {
        uint8_t byte = (uint8_t)read_rd_c(FS_get_rdaddr(), header_size + binary_file->base_size.base + titan_pos);
        titan_pos++;
        buffer[_i] = byte;
    }

    return *(uint32_t *)buffer;
}

uint32_t TITAN_read_uint32_be(unsigned int_size) {
    uint8_t buffer[int_size];
    unsigned header_size = FS_get_hsize();
    for (unsigned _i = 0; _i < int_size; _i++) {
        uint8_t byte = (uint8_t)read_rd_c(FS_get_rdaddr(), header_size + binary_file->base_size.base + titan_pos);
        titan_pos++;
        buffer[3 - _i] = byte;  // Reverse the order of the bytes
    }
    return *(uint32_t *)buffer;
}

uint16_t TITAN_read_uint16(unsigned int_size) {
    uint8_t buffer[int_size];
    unsigned header_size = FS_get_hsize();
    for (unsigned _i=0; _i<int_size; _i++) {
        uint8_t byte = (uint8_t)read_rd_c(FS_get_rdaddr(), header_size+binary_file->base_size.base+titan_pos);
        titan_pos++;
        buffer[_i] = byte;
    }
    return *(uint16_t*)buffer;
}

uint8_t TITAN_read_uint8(unsigned int_size) {
    uint8_t buffer[int_size];
    unsigned header_size = FS_get_hsize();
    for (unsigned _i=0; _i<int_size; _i++) {
        uint8_t byte = (uint8_t)read_rd_c(FS_get_rdaddr(), header_size+binary_file->base_size.base+titan_pos);
        titan_pos++;
        buffer[_i] = byte;
    }
    return *(uint8_t*)buffer;
}

char *TITAN_read_string(unsigned str_size) {
    char *buffer = memalloc(str_size+2);
    unsigned header_size = FS_get_hsize();
    for (unsigned _i=0; _i<str_size; _i++) {
        char byte = read_rd_c(FS_get_rdaddr(), header_size+binary_file->base_size.base+titan_pos);
        titan_pos++;
        buffer[_i] = byte;
    }
    return buffer;
}

uint16_t TITAN_read_short() {
    uint8_t byte1 = TITAN_read_uint8(1);
    uint8_t byte2 = TITAN_read_uint8(1);
    uint16_t res = (byte2 << 8) | byte1;
    if (res>255) { // Can't get shifting to work here
        return res/256;
    }
    return res;
}

void TITAN_reset_pos() {
    titan_pos =0;
}

void TITAN_set_file(File* source) {
    binary_file = source;
}
