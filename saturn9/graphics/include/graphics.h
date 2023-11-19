#pragma once

#include <limine.h>

#ifndef GRAPHICS_H
#define GRAPHICS_H

extern struct limine_framebuffer_request framebuffer_request;

void draw_pixel(unsigned x, unsigned y, unsigned color);
void draw_char(unsigned x, unsigned y, char character, unsigned color, unsigned scale);
void delete_last();
void putchar(char character, unsigned color, unsigned scale);
void draw_screen(unsigned color);
void framebuffer_init();

void set_x_offset(unsigned value);
void set_y_offset(unsigned value);
unsigned get_x_offset();
unsigned get_y_offset();
void set_background(unsigned value);
unsigned get_background();
void set_foreground(unsigned value);
unsigned get_foreground();
void set_fontsize(unsigned value);
unsigned get_fontsize();

void printstr(const char* string, unsigned color, unsigned scale);
void print_at(const char* string, unsigned x, unsigned y, unsigned color, unsigned scale);

#endif
