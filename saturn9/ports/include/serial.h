#include <portio.h>
#ifndef SERIAL_H
#define SERIAL_H

#define PORT 0x3f8

int serial_init();

int is_transmit_empty();

void write_serial_ch(char ch);

void write_serial(const char* str);

#endif