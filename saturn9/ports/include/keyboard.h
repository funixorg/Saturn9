#include <stdint.h>
#include <stdbool.h>

#ifndef KEYBOARD_H
#define KEYBOARD_H

typedef struct {
    char key;
    bool shift;
    bool ctrl;
    bool modifier;
    bool capslock;
    bool alt;
    char buffer[512];
    unsigned bindex;
} KeyboardInfo;

void parse_key(uint8_t key, char layout[], char shift_layout[]);

char *read_keyboard();
char readkey();
void setkey(char key);

#endif
