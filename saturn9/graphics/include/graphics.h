#pragma once

#include <limine.h>

#ifndef GRAPHICS_H
#define GRAPHICS_H

#define DEFAULT_X 0
#define DEFAULT_Y 4

extern struct limine_framebuffer_request framebuffer_request;

void GP_draw_pixel(unsigned x, unsigned y, unsigned color);
void GP_draw_char(unsigned x, unsigned y, char character, unsigned color, unsigned scale);
void GP_draw_rectangle(unsigned x, unsigned y, unsigned width, unsigned height, unsigned color);
void GP_draw_screen(unsigned color);
void GP_framebuffer_init();

unsigned GP_get_screen_width();
unsigned GP_get_screen_height();

#endif
