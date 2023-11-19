#include <graphics.h>

#include <mem.h>
#include <common.h>
#include <stdlib.h>

void _start(void) {
    framebuffer_init();

    set_background(0x0e0e0e);
    set_foreground(0x3c3c3c);
    set_fontsize(2);

    draw_screen(get_background());
    char *name = "Unity";
    int hex = 0xfff2a;
    printf("Hello #{0x55555}%s #{0xfffff}${0xff0000}%x\n", name, hex);
    hcf();
}