#include <stdbool.h>
#include <stddef.h>
#include <limine.h>

#include <graphics.h>
#include <terminal.h>
#include <stdlib.h>
#include <common.h>

LIMINE_BASE_REVISION(1);

struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

struct limine_framebuffer *framebuffer;
unsigned screen_width;
unsigned screen_height;

void GP_draw_pixel(unsigned x, unsigned y, unsigned color) {
    uint32_t *fb_ptr = framebuffer->address;
    fb_ptr[y * (framebuffer->pitch / 4) + x] = color;
}

void GP_draw_rectangle(unsigned x, unsigned y, unsigned width, unsigned height, unsigned color) {
    for (unsigned i = 0; i < height; ++i) {
        for (unsigned j = 0; j < width; ++j) {
            GP_draw_pixel(x + j, y + i, color);
        }
    }
}


void GP_draw_screen(unsigned color) {
    unsigned width = framebuffer->width;
    unsigned height = framebuffer->height;
    unsigned x=0;
    unsigned y=0;

    while (y<height) {
        while (x<width) {
            GP_draw_pixel(x,y,color);
            x++;
        }
        x=0;
        y++;
    }
    TERM_draw_bar();
}

void GP_framebuffer_init() {
    if (LIMINE_BASE_REVISION_SUPPORTED == false) {
        hcf();
    }

    // Ensure we got a framebuffer.
    if (framebuffer_request.response == NULL
    || framebuffer_request.response->framebuffer_count < 1) {
        hcf();
    }

    framebuffer = framebuffer_request.response->framebuffers[0];
    screen_width = framebuffer->width;
    screen_height = framebuffer->height;
    TERM_set_ypos(DEFAULT_Y);
    TERM_set_xpos(DEFAULT_X);
}

unsigned GP_get_screen_width() {
    return screen_width;
}

unsigned GP_get_screen_height() {
    return screen_height;
}