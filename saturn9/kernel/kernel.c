#include <graphics.h>

#include <mem.h>
#include <common.h>
#include <stdlib.h>
#include <idt.h>
#include <exceptions.h>
#include <serial.h>
#include <pic.h>
#include <stdio.h>
#include <terminal.h>
#include <gdt.h>
#include <irqs.h>
#include <keyboard.h>
#include <pmm.h>
#include <rhea.h>
#include <initrd.h>


void _start(void) {
    sys_init_fpu();

    framebuffer_init();
    serial_init();

    set_background(0x0e0e0e);
    set_foreground(0x6d6d6d);
    set_fontsize(1);
    draw_screen(get_background());

    printf_serial("[OK] Serial\n");

    /*printf("[...] #{0xffc0cb}GDT");
    gdt_init();
    while (get_x_offset()>0) { delete_last(); }
    printf("[#{0x00ff00}OK#{0x6d6d6d}] #{0xffc0cb}GDT\n");*/

    printf("[...] #{0xffc0cb}IDT");
    idt_init();
    while (get_x_offset()>0) { delete_last(); }
    printf("[#{0x00ff00}OK#{0x6d6d6d}] #{0xffc0cb}IDT\n");

    printf("[...] #{0xffc0cb}PIT");
    i8259_Configure(PIC_REMAP_OFFSET, PIC_REMAP_OFFSET + 8, false);
    pit_init();
    while (get_x_offset()>0) { delete_last(); }
    printf("[#{0x00ff00}OK#{0x6d6d6d}] #{0xffc0cb}PIT\n");

    printf("[...] #{0xffc0cb}ROUTINE");
    init_periodic_event();
    while (get_x_offset()>0) { delete_last(); }
    printf("[#{0x00ff00}OK#{0x6d6d6d}] #{0xffc0cb}ROUTINE\n");

    printf("[...] #{0xffc0cb}EXCEPTIONS");
    init_os_interupts();
    while (get_x_offset()>0) { delete_last(); }
    printf("[#{0x00ff00}OK#{0x6d6d6d}] #{0xffc0cb}EXCEPTIONS\n");

    printf("[...] #{0xffc0cb}STACK");
    while (get_x_offset()>0) { delete_last(); }
    printf("[#{0x00ff00}OK#{0x6d6d6d}] #{0xffc0cb}STACK %s\n", formatBytes(STACK_SIZE));

    printf("[...] #{0xffc0cb}MEMORY");
    while (get_x_offset()>0) { delete_last(); }
    printf("[#{0x00ff00}OK#{0x6d6d6d}] #{0xffc0cb}MEMORY %s\n", formatBytes(get_avaiable_mem()));

    printf("[...] #{0xffc0cb}LVL4-PAGING");
    while (get_x_offset()>0) { delete_last(); }
    printf("[#{0x00ff00}OK#{0x6d6d6d}] #{0xffc0cb}LVL4-PAGING\n");

    printf("[...] #{0xffc0cb}CPU");
    while (get_x_offset()>0) { delete_last(); }
    printf("[#{0x00ff00}OK#{0x6d6d6d}] #{0xffc0cb}%d CPUs\n", get_cpu_count());

    printf("[...] #{0xffc0cb}BOOT_TIME");
    while (get_x_offset()>0) { delete_last(); }
    printf("[#{0x00ff00}OK#{0x6d6d6d}] #{0xffc0cb}BOOT_TIME %s\n", unix_to_time(get_boot_time()));

    printf("[...] #{0xffc0cb}KERNEL_ADDR");
    while (get_x_offset()>0) { delete_last(); }
    printf("[#{0x00ff00}OK#{0x6d6d6d}] #{0xffc0cb}KERNEL_ADDR %x\n", get_kernel_addr());

    printf("[...] #{0xffc0cb}INITRD");
    initramdisk();
    while (get_x_offset()>0) { delete_last(); }
    printf("[#{0x00ff00}OK#{0x6d6d6d}] #{0xffc0cb}INITRD\n");

    pit_sleep(10);

    printf("[...] #{0xffc0cb}KEYBOARD");
    keyboard_init();
    while (get_x_offset()>0) { delete_last(); }
    printf("[#{0x00ff00}OK#{0x6d6d6d}] #{0xffc0cb}KEYBOARD\n");

    rhea_proc("0x813 hello\n\"Hello World\"\n()");

    terminal_init();
    /*for (;;) {
        routine();
    }*/
}
