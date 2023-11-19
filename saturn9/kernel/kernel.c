#include <graphics.h>

#include <mem.h>
#include <common.h>
#include <stdlib.h>
#include <idt.h>
#include <exceptions.h>
#include <stack.h>
#include <serial.h>
#include <syscalls.h>
#include <pic.h>
#include <stdio.h>
#include <terminal.h>


void routine() {
  shell();
}

void _start(void) {
    framebuffer_init();
    serial_init();
    terminal_init();

    set_background(0x0e0e0e);
    set_foreground(0x3c3c3c);
    set_fontsize(2);
    draw_screen(get_background());

    init_idt_64();
    stack_init();
    set_idt_descriptor_64(0, div_by_0_handler, TRAP_GATE_FLAGS);
    set_idt_descriptor_64(0x80, syscall_dispatcher, TRAP_GATE_FLAGS);

    disable_pic();
    remap_pic();

    __asm__ __volatile__("sti");

    for (;;) {
        routine();
    }
}
