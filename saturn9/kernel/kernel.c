#include <graphics.h>

#include <mem.h>
#include <common.h>
#include <stdlib.h>

void _start(void) {
    framebuffer_init();

    set_background(0x000000);
    set_foreground(0xffff);
    set_fontsize(2);

    draw_screen(get_background());
    //printstr("Hello World!", 0xfffff, 2);
    char *name = "Unity";
    char *name1 = "Github";
    printf("Hello %s\nHello %s", name, name1);
    hcf();
}