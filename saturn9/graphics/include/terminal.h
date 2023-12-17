
#include <graphics.h>
#ifndef TERMINAL_H
#define TERMINAL_H

void TERM_delete_last();
void TERM_putchar(char character, unsigned color, unsigned scale);

void TERM_draw_cursor();
void TERM_delete_cursor();

void TERM_set_background(unsigned value);
unsigned TERM_get_background();
void TERM_set_foreground(unsigned value);
unsigned TERM_get_foreground();
void TERM_set_fontsize(unsigned value);
unsigned TERM_get_fontsize();

void TERM_printstr(const char* string, unsigned color, unsigned scale);

unsigned TERM_get_xpos();
unsigned TERM_get_ypos();
void TERM_set_xpos(unsigned value);
void TERM_set_ypos(unsigned value);

void TERM_draw_bar();
void TERM_scroll_up();

#endif