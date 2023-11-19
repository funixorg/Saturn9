#include <keyboard.h>
#include <portio.h>
#include <serial.h>
#include <keyboardlayouts.h>
#include <stdlib.h>
#include <stdio.h>
#include <mem.h>

KeyboardInfo keyboard_info;

void parse_key(uint8_t key, char layout[], char shift_layout[]) {
    if      (key==1)  {return;}
    else if (key==58) {
        if (keyboard_info.capslock) { keyboard_info.capslock = false; }
        else { keyboard_info.capslock = true; }
        return;
    }
    else if (key==42) { keyboard_info.shift=true; return;}
    else if (key==54) { keyboard_info.shift=true; return;}
    else if (key==29) { keyboard_info.ctrl=true; return;}
    else if (key==91) { keyboard_info.modifier=true; return;}
    else if (key==56) { keyboard_info.alt=true; return;}
    else if (key==93) { }

    else {
        if (key<=strlen(layout)) { 
            if (keyboard_info.shift) { keyboard_info.key = shift_layout[key-1]; }
            else { keyboard_info.key = layout[key-1]; }
            
            if (keyboard_info.key == '\b' && keyboard_info.bindex>0) {
              keyboard_info.bindex--;
              keyboard_info.buffer[keyboard_info.bindex] = '\0';
              printf("\b");
            } else {
                printf("%s", chtostr(keyboard_info.key));
                if (keyboard_info.key != '\n') { keyboard_info.buffer[keyboard_info.bindex] = keyboard_info.key; }
                keyboard_info.bindex++;
            }
        }
    }
}

void parse_release(uint8_t key) {
    switch (key) {
        case (170): { keyboard_info.shift=false; }
        case (182) : { keyboard_info.shift=false; }
        case (157) : { keyboard_info.shift=false; }
        case (219) : { keyboard_info.shift=false; }
        case (184) : { keyboard_info.shift=false; }
        default: {}
    }
}

char *read_keyboard() {
    keyboard_info.bindex =0;
    memset(keyboard_info.buffer, 0, sizeof(keyboard_info.buffer));
    while (keyboard_info.key != '\n') {
      if (inb(0x64) & 0x1) {
          uint8_t key = inb(0x60);
          if (key < 0x80) {
              parse_key(key, en_US_layout, en_US_shift_layout);
          } else { parse_release(key); }
      }
  }
  keyboard_info.key='\0';
  return keyboard_info.buffer;
}

void setkey(char key) {
    keyboard_info.key = key;
}
