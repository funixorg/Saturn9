#include <graphics.h>
#include <terminal.h>
#include <fonts.h>

unsigned background;
unsigned foreground;
unsigned font_size;

unsigned xpos=0;
unsigned ypos=0;

void TERM_delete_last() {
    xpos--;

    unsigned start_x = xpos * (font_width * TERM_get_fontsize());
    unsigned start_y = (ypos * (font_height * TERM_get_fontsize())/2);

    for (int i = 0; i < font_height * TERM_get_fontsize(); ++i) {
        for (int j = 0; j < font_width * TERM_get_fontsize(); ++j) {
            GP_draw_pixel(start_x + j, start_y + i, TERM_get_background());
        }
    }
}

void TERM_putchar(char character, unsigned color, unsigned scale) {
    unsigned start_x = xpos * (font_width * scale);
    unsigned start_y = (ypos * (font_height * scale)/2);

    if (character == '\n') {
        xpos = DEFAULT_X;
        ypos += 2;
        return;
    }
    else if (character == '\b') {
        TERM_delete_last();
        return;
    }
    else if (character== '\t') {
        TERM_printstr("    ", color, scale);
        return;
    }

    if (start_x + font_width * scale >= GP_get_screen_width()) {
        xpos = DEFAULT_X;
        ypos += 2;
        start_x = xpos * (font_width * scale);
        start_y = (ypos * (font_height * scale)/2);
    }

    if (start_y + font_height * scale >= GP_get_screen_height()) {
        TERM_scroll_up();
        start_x = xpos * (font_width * scale);
        start_y = (ypos * (font_height * scale))/2;
    }

    GP_draw_rectangle(start_x, start_y, font_width*scale, font_height*scale, TERM_get_background());

    for (int i = 0; i < font_height; ++i) {
        for (int j = 0; j < font_width; ++j) {
            if ((font[character][i] >> (font_width - 1 - j)) & 1) {
                for (int si = 0; si < scale; ++si) {
                    for (int sj = 0; sj < scale; ++sj) {
                        GP_draw_pixel(start_x + j * scale + sj, start_y + i * scale + si, color);
                    }
                }
            }
        }
    }
    xpos++;
}

void TERM_draw_cursor() {
    TERM_putchar('_', TERM_get_foreground(), TERM_get_fontsize());
    xpos--;
}

void TERM_delete_cursor() {
    TERM_putchar('_', TERM_get_background(), TERM_get_fontsize());
    xpos--;
}

void TERM_set_background(unsigned value) {
    background=value;
}

unsigned TERM_get_background() {
    return background;
}

void TERM_set_foreground(unsigned value) {
    foreground=value;
}

unsigned TERM_get_foreground() {
    return foreground;
}

void TERM_set_fontsize(unsigned value) {
    font_size=value;
}

unsigned TERM_get_fontsize() {
    return font_size;
}


void TERM_printstr(const char* string, unsigned color, unsigned scale) {
    unsigned i=0;
    while (string[i] != '\0') {
        TERM_putchar(string[i], color, scale);
        i++;
    }
}

unsigned TERM_get_xpos() {
    return xpos;
}

unsigned TERM_get_ypos() {
    return ypos;
}

void TERM_set_xpos(unsigned value) {
    xpos = value;
}

void TERM_set_ypos(unsigned value) {
    ypos = value;
}


void TERM_scroll_up() {
    GP_draw_screen(TERM_get_background());
    TERM_set_ypos(DEFAULT_Y);
    TERM_set_xpos(DEFAULT_X);
}

void TERM_draw_bar() {
    GP_draw_rectangle(0,0, GP_get_screen_width(), font_height*TERM_get_fontsize()+6, 0x222222);
}