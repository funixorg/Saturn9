#include <stdint.h>
#include <filesystem.h>

#ifndef TITAN_READER
#define TITAN_READER

uint32_t TITAN_read_uint32(unsigned int_size);
uint32_t TITAN_read_uint32_be(unsigned int_size);
uint16_t TITAN_read_uint16(unsigned int_size);
uint8_t TITAN_read_uint8(unsigned int_size);
char *TITAN_read_string(unsigned str_size);
uint16_t TITAN_read_short();

void TITAN_reset_pos();
void TITAN_set_file(File* source);

#endif