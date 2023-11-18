#include <stdbool.h>
#include <stddef.h>
#include <limine.h>

#include <graphics.h>
#include <fonts.h>
#include <common.h>

LIMINE_BASE_REVISION(1);

struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

struct limine_framebuffer *framebuffer;

unsigned xpos=0;
unsigned ypos=0;
unsigned background;
unsigned foreground;
unsigned font_size;


void draw_pixel(unsigned x, unsigned y, unsigned color) {
    uint32_t *fb_ptr = framebuffer->address;
    fb_ptr[y * (framebuffer->pitch / 4) + x] = color;
}

void draw_char(unsigned x, unsigned y, char character, unsigned color, unsigned scale) {
    unsigned char_width = font_width * scale;
    unsigned char_height = font_height * scale;

    unsigned start_x = x * char_width;
    unsigned start_y = y * char_height;

    if (character == '\n') {
        x = 0;
        y++;
        return;
    }

    for (int i = 0; i < font_height; ++i) {
        for (int j = 0; j < font_width; ++j) {
            if ((font[character][i] >> (font_width - 1 - j)) & 1) {
                for (int si = 0; si < scale; ++si) {
                    for (int sj = 0; sj < scale; ++sj) {
                        draw_pixel(start_x + j * scale + sj, start_y + i * scale + si, color);
                    }
                }
            }
        }
    }
}

void putchar(char character, unsigned color, unsigned scale) {
    unsigned start_x = xpos * (font_width * scale);
    unsigned start_y = ypos * (font_height * scale);
    unsigned screen_width = framebuffer->width;

    if (character == '\n') {
        xpos = 0;
        ypos += 2;
        return;
    }

    if (start_x + font_width * scale >= screen_width) {
        xpos = 0;
        ypos += 2;
        start_x = xpos * (font_width * scale);
        start_y = ypos * (font_height * scale);
    }

    for (int i = 0; i < font_height; ++i) {
        for (int j = 0; j < font_width; ++j) {
            if ((font[character][i] >> (font_width - 1 - j)) & 1) {
                for (int si = 0; si < scale; ++si) {
                    for (int sj = 0; sj < scale; ++sj) {
                        draw_pixel(start_x + j * scale + sj, start_y + i * scale + si, color);
                    }
                }
            }
        }
    }
    xpos++;
}

void draw_screen(unsigned color) {
    unsigned width = framebuffer->width;
    unsigned height = framebuffer->height;
    unsigned x=0;
    unsigned y=0;

    while (y<height) {
        while (x<width) {
            draw_pixel(x,y,color);
            x++;
        }
        x=0;
        y++;
    }
}

void framebuffer_init() {
    if (LIMINE_BASE_REVISION_SUPPORTED == false) {
        hcf();
    }

    // Ensure we got a framebuffer.
    if (framebuffer_request.response == NULL
    || framebuffer_request.response->framebuffer_count < 1) {
        hcf();
    }

    framebuffer = framebuffer_request.response->framebuffers[0];
    set_y_offset(get_y_offset()+1);
}


void set_x_offset(unsigned value) {
    xpos = value;
}

void set_y_offset(unsigned value) {
    ypos = value;
}

unsigned get_x_offset() {
    return xpos;
}

unsigned get_y_offset() {
    return ypos;
}

void set_background(unsigned value) {
    background=value;
}

unsigned get_background() {
    return background;
}

void set_foreground(unsigned value) {
    foreground=value;
}

unsigned get_foreground() {
    return foreground;
}

void set_fontsize(unsigned value) {
    font_size=value;
}

unsigned get_fontsize() {
    return font_size;
}


void printstr(const char* string, unsigned color, unsigned scale) {
    char modifiedString[2048];
    to_upper(modifiedString, string);

    unsigned i=0;
    while (modifiedString[i] != '\0') {
        putchar(modifiedString[i], color, scale);
        i++;
    }
}

void print_at(const char* string, unsigned x, unsigned y, unsigned color, unsigned scale) {
    char modifiedString[2048];
    to_upper(modifiedString, string);

    unsigned xi = x;

    unsigned i = 0;
    while (modifiedString[i] != '\0') {
        draw_char(xi, y, modifiedString[i], color, scale);
        xi++;
        i++;
    }
}