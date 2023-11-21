#include <graphics.h>

#include <mem.h>
#include <common.h>
#include <stdlib.h>
#include <idt.h>
#include <exceptions.h>
#include <stack.h>
#include <serial.h>
#include <pic.h>
#include <stdio.h>
#include <terminal.h>
#include <gdt.h>
#include <irqs.h>
#include <keyboard.h>

void routine() {
  shell();
}

void _start(void) {
    sys_init_fpu();

    framebuffer_init();
    serial_init();

    set_background(0x0e0e0e);
    set_foreground(0x3c3c3c);
    set_fontsize(1);
    draw_screen(get_background());

    printf_serial("[OK] Serial\n");

    printf("[...] #{0xffc0cb}IDT");
    idt_init();
    while (get_x_offset()>0) { delete_last(); }
    printf("[#{0x00ff00}OK#{0x3c3c3c}] #{0xffc0cb}IDT\n");

    printf("[...] #{0xffc0cb}PIT");
    i8259_Configure(PIC_REMAP_OFFSET, PIC_REMAP_OFFSET + 8, false);
    pit_init();
    while (get_x_offset()>0) { delete_last(); }
    printf("[#{0x00ff00}OK#{0x3c3c3c}] #{0xffc0cb}PIT\n");

    printf("[...] #{0xffc0cb}ROUTINE");
    init_periodic_event();
    while (get_x_offset()>0) { delete_last(); }
    printf("[#{0x00ff00}OK#{0x3c3c3c}] #{0xffc0cb}ROUTINE\n");

    printf("[...] #{0xffc0cb}STACK");
    stack_init();
    while (get_x_offset()>0) { delete_last(); }
    printf("[#{0x00ff00}OK#{0x3c3c3c}] #{0xffc0cb}STACK\n");

    printf("[...] #{0xffc0cb}EXCEPTIONS");
    init_os_interupts();
    while (get_x_offset()>0) { delete_last(); }
    printf("[#{0x00ff00}OK#{0x3c3c3c}] #{0xffc0cb}EXCEPTIONS\n");

    pit_sleep(10);

    printf("[...] #{0xffc0cb}KEYBOARD");
    keyboard_init();
    while (get_x_offset()>0) { delete_last(); }
    printf("[#{0x00ff00}OK#{0x3c3c3c}] #{0xffc0cb}KEYBOARD\n");

    terminal_init();
    /*for (;;) {
        routine();
    }*/
}
