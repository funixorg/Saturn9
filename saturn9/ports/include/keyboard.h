#include <stdint.h>
#include <stdbool.h>

#ifndef KEYBOARD_H
#define KEYBOARD_H

#define PS2_COMMAND 0x64
#define PS2_DATA 0x60

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

char readkey();
void setkey(char value);
void clearbuffer();
char *readbuffer();

void keyboard_init();

#endif
