#include <graphics.h>

#include <mem.h>
#include <common.h>

void _start(void) {
    framebuffer_init();
    draw_screen(0x000000);
    printstr("Hello World!", 0xfffff, 2);
    hcf();
}