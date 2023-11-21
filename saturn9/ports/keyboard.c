#include <keyboard.h>
#include <portio.h>
#include <serial.h>
#include <keyboardlayouts.h>
#include <stdlib.h>
#include <stdio.h>
#include <mem.h>
#include <pic.h>
#include <idt.h>

KeyboardInfo keyboard_info;

void parse_key(uint8_t key, char layout[], char shift_layout[]) {
    if (key == 1) {
        return;
    } else if (key == 58) {
        if (keyboard_info.capslock) {
            keyboard_info.capslock = false;
        } else {
            keyboard_info.capslock = true;
        }
        return;
    } else if (key == 42) {
        keyboard_info.shift = true;
        return;
    } else if (key == 54) {
        keyboard_info.shift = true;
        return;
    } else if (key == 29) {
        keyboard_info.ctrl = true;
        return;
    } else if (key == 91) {
        keyboard_info.modifier = true;
        return;
    } else if (key == 56) {
        keyboard_info.alt = true;
        return;
    } else if (key == 93) {
        // Do nothing for now
    } else {
        if (key <= strlen(layout)) {
            if (keyboard_info.shift) {
                keyboard_info.key = shift_layout[key - 1];
                keyboard_info.buffer[keyboard_info.bindex] = shift_layout[key - 1];
            } else {
                keyboard_info.key = layout[key - 1];
                keyboard_info.buffer[keyboard_info.bindex] = layout[key - 1];
            }
            printf("%s", chtostr(keyboard_info.key));

            if (keyboard_info.buffer[keyboard_info.bindex] == '\b' && keyboard_info.bindex > 0) {
                keyboard_info.bindex--;
            } else {
                keyboard_info.bindex++;
            }

            // Null-terminate the buffer
            keyboard_info.buffer[keyboard_info.bindex] = '\0';
        }
    }
}

void parse_release(uint8_t key) {
    switch (key) {
        case (170) : { keyboard_info.shift=false; break; }
        case (182) : { keyboard_info.shift=false; break; }
        case (157) : { keyboard_info.ctrl=false; break; }
        case (219) : { keyboard_info.modifier=false; break; }
        case (184) : { keyboard_info.alt=false; break; }
        default: {}
    }
}

char readkey() {
    return keyboard_info.key;
}

void setkey(char value) {
    keyboard_info.key = value;
}

void clearbuffer() {
    memset(keyboard_info.buffer, 0, sizeof(keyboard_info.buffer));
    keyboard_info.bindex=0;
}

char *readbuffer() {
    keyboard_info.buffer[keyboard_info.bindex-1] = '\0';
    return keyboard_info.buffer;
}

__attribute__((interrupt)) void keyboard_handler(void *)
{
    uint8_t data = inb(PS2_DATA);

    if (data == 0) {
        i8259_SendEndOfInterrupt(1);
        return;
    }

    if (data < 0x80) {
        parse_key(data, en_US_layout, en_US_shift_layout);
        keyboard_info.buffer[keyboard_info.bindex] = keyboard_info.key;
    } else { parse_release(data); }

    i8259_SendEndOfInterrupt(1);
}

void keyboard_init() {
    keyboard_info.bindex=0;
    set_idt_gate(0x21, (uint64_t)&keyboard_handler, 0x28, 0x8E);

    if (inb(PS2_COMMAND) & 0x1)
        inb(PS2_DATA);
    outb(PS2_COMMAND, 0x60);
    outb(PS2_DATA, 0b01100111);

    outb(PS2_COMMAND, 0xAE);
    outb(PS2_DATA, 0xf4);
    i8259_Unmask(1);
}