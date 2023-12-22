#include <graphics.h>
#include <terminal.h>
#include <shell.h>

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
#include <mimas.h>
#include <titan.h>


void _start(void) {
    sys_init_fpu();

    GP_framebuffer_init();
    serial_init();

    TERM_set_background(0x0e0e0e);
    TERM_set_foreground(0x6d6d6d);
    TERM_set_fontsize(1);
    GP_draw_screen(TERM_get_background());

    printf_serial("[OK] Serial\n");

    /*printf("[...] #{0xffc0cb}GDT");
    gdt_init();
    while (TERM_get_xpos()>0) { TERM_delete_last(); }
    printf("[#{0x00ff00}OK#{0x6d6d6d}] #{0xffc0cb}GDT\n");*/

    printf("[...] #{0xffc0cb}IDT");
    idt_init();
    while (TERM_get_xpos()>0) { TERM_delete_last(); }
    printf("[#{0x00ff00}OK#{0x6d6d6d}] #{0xffc0cb}IDT\n");

    printf("[...] #{0xffc0cb}PIT");
    i8259_Configure(PIC_REMAP_OFFSET, PIC_REMAP_OFFSET + 8, false);
    pit_init();
    while (TERM_get_xpos()>0) { TERM_delete_last(); }
    printf("[#{0x00ff00}OK#{0x6d6d6d}] #{0xffc0cb}PIT\n");

    printf("[...] #{0xffc0cb}ROUTINE");
    init_periodic_event();
    while (TERM_get_xpos()>0) { TERM_delete_last(); }
    printf("[#{0x00ff00}OK#{0x6d6d6d}] #{0xffc0cb}ROUTINE\n");

    printf("[...] #{0xffc0cb}EXCEPTIONS");
    init_os_interupts();
    while (TERM_get_xpos()>0) { TERM_delete_last(); }
    printf("[#{0x00ff00}OK#{0x6d6d6d}] #{0xffc0cb}EXCEPTIONS\n");

    printf("[...] #{0xffc0cb}STACK");
    while (TERM_get_xpos()>0) { TERM_delete_last(); }
    printf("[#{0x00ff00}OK#{0x6d6d6d}] #{0xffc0cb}STACK %s\n", formatBytes(STACK_SIZE));

    printf("[...] #{0xffc0cb}MEMORY");
    while (TERM_get_xpos()>0) { TERM_delete_last(); }
    printf("[#{0x00ff00}OK#{0x6d6d6d}] #{0xffc0cb}MEMORY %s\n", formatBytes(get_avaiable_mem()));

    printf("[...] #{0xffc0cb}LVL4-PAGING");
    while (TERM_get_xpos()>0) { TERM_delete_last(); }
    printf("[#{0x00ff00}OK#{0x6d6d6d}] #{0xffc0cb}LVL4-PAGING\n");

    printf("[...] #{0xffc0cb}CPU");
    while (TERM_get_xpos()>0) { TERM_delete_last(); }
    printf("[#{0x00ff00}OK#{0x6d6d6d}] #{0xffc0cb}%d CPUs\n", get_cpu_count());

    printf("[...] #{0xffc0cb}BOOT_TIME");
    while (TERM_get_xpos()>0) { TERM_delete_last(); }
    printf("[#{0x00ff00}OK#{0x6d6d6d}] #{0xffc0cb}BOOT_TIME %s\n", unix_to_time(get_boot_time()));

    printf("[...] #{0xffc0cb}KERNEL_ADDR");
    while (TERM_get_xpos()>0) { TERM_delete_last(); }
    printf("[#{0x00ff00}OK#{0x6d6d6d}] #{0xffc0cb}KERNEL_ADDR %x\n", get_kernel_addr());

    printf("[...] #{0xffc0cb}INITRD");
    initramdisk();
    while (TERM_get_xpos()>0) { TERM_delete_last(); }
    printf("[#{0x00ff00}OK#{0x6d6d6d}] #{0xffc0cb}INITRD\n");

    pit_sleep(10);

    printf("[...] #{0xffc0cb}KEYBOARD");
    keyboard_init();
    while (TERM_get_xpos()>0) { TERM_delete_last(); }
    printf("[#{0x00ff00}OK#{0x6d6d6d}] #{0xffc0cb}KEYBOARD\n");

    printf("\n\n");

    //char *content = read_path("/commands/clear.re");
    /*FileList *files = list_dir("/");
    for (unsigned _i=0; _i<files->file_count; _i++) {
        printf_serial("PTH: %s\n", files->paths[_i]);
    }*/
    //File *file_handle = VFS_get_file("/img/saturn.mas");
    //MIM_display_mimimg(file_handle, 250,250, 1, 1);


    test_titan();

    //if (content) { rhea_proc(content); }
    //else { printf_serial("FAIL!\n"); }

    SHELL_shell_init();
    /*for (;;) {
        routine();
    }*/
}
