#include <idt.h>
#include <stdio.h>
#include <pit.h>
#include <pic.h>
#include <terminal.h>
#include <common.h>

#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

// (0-31 in IDT)

typedef struct {
	uint64_t r15;
	uint64_t r14;
	uint64_t r13;
	uint64_t r12;
	uint64_t r11;
	uint64_t r10;
	uint64_t r9;
	uint64_t r8;
	uint64_t rbp;
	uint64_t rdi;
	uint64_t rsi;
	uint64_t rdx;
	uint64_t rcx;
	uint64_t rbx;
	uint64_t rax;

	uint64_t vector;
	uint64_t err;

	uint64_t rip;
	uint64_t cs;
	uint64_t rflags;
	uint64_t rsp;
	uint64_t ss;
} __attribute__((packed)) int_frame_t;

void dump_idt_entries() {
    idt_entry_t *idt_ptr = get_idt();
    for (int i = 0; i < 4; i++) {
        printf("IDT Entry %d:\n", i);
        printf("  Base Low: %x\n", idt_ptr[i].offset_low);
        printf("  Segment Selector: %x\n", idt_ptr[i].selector);
        printf("  Flags: %x\n", idt_ptr[i].flags);
        printf("  Base High: %x\n", idt_ptr[i].offset_high);
        printf("\n");
    }
}


void dump_idt_register() {
    uint64_t idt_register;
    asm volatile ("sidt %0" : "=m"(idt_register));
    printf("IDT Register:\n");
    printf("  Base Address: %x\n", idt_register & 0xFFFFFFFFFFFF);
    printf("  Limit: %x\n", (idt_register >> 48) & 0xFFFF);
}

void* get_isr_address(int interrupt_number) {
    void* isr_address;
    asm volatile (
        "call 1f\n"
        "1: pop %0"
        : "=r" (isr_address)
    );
    return (void*)((uintptr_t)isr_address + interrupt_number * 16);
}

void dump_isr_addresses() {
    for (int i = 0; i < 16; i++) {
        printf("ISR Address for Interrupt %d: %x\n", i, (uint64_t)get_isr_address(i));
    }
    printf("\n");
}

void dump_registers(int_frame_t frame) {
    printf("RAX: %x     RBX: %x     RCX: %x\n", frame.rax, frame.rbx, frame.rcx);
    printf("RBP: %x     RDI: %x     RDX: %x\n", frame.rbp, frame.rdi, frame.rdx);
    printf("ERR: %x     RSP: %x     RIP: %x\n", frame.err, frame.rsp, frame.rip);
    printf("CS : %x     SS : %x     RF : %x\n", frame.cs, frame.ss, frame.rflags);
}


void division_by_zero_error(int_frame_t frame) {
    TERM_set_background(0xff0000);
    TERM_scroll_up();
    printf("#{0x00ff00}====== #{0xfffff} >__< Division by zero!  #{0x00ff00}======#{0xfffff}\n");
    dump_registers(frame);
    dump_idt_register();
    dump_idt_entries();
    printf("\\@RES");
    hcf();
}

void single_step_eror(int_frame_t frame) {
    TERM_set_background(0xff0000);
    TERM_scroll_up();
    printf("#{0x00ff00}====== #{0xfffff} >__< Single_step int!  #{0x00ff00}======#{0xfffff}\n");
    dump_registers(frame);
    dump_idt_register();
    dump_idt_entries();
    printf("\\@RES");
    hcf();
}

void overflow_eror(int_frame_t frame) {
    TERM_set_background(0xff0000);
    TERM_scroll_up();
    printf("#{0x00ff00}====== #{0xfffff} >__< Overflow error!  #{0x00ff00}======#{0xfffff}\n");
    dump_registers(frame);
    dump_idt_register();
    dump_idt_entries();
    printf("\\@RES");
    hcf();
}

void bound_error(int_frame_t frame) {
    TERM_set_background(0xff0000);
    TERM_scroll_up();
    printf("#{0x00ff00}====== #{0xfffff} >__< Bound range exceeded error!  #{0x00ff00}======#{0xfffff}\n");
    dump_registers(frame);
    dump_idt_register();
    dump_idt_entries();
    printf("\\@RES");
    hcf();
}

void double_fault(int_frame_t frame) {
    TERM_set_background(0xff0000);
    TERM_scroll_up();
    printf("#{0x00ff00}====== #{0xfffff} >__< Double fault error!  #{0x00ff00}======#{0xfffff}\n");
    dump_registers(frame);
    dump_idt_register();
    dump_idt_entries();
    printf("\\@RES");
    hcf();
}

void init_os_interupts() {
    set_idt_gate(0, (uint64_t)&division_by_zero_error, 0x28, 0x8E);
    set_idt_gate(1, (uint64_t)&single_step_eror, 0x28, 0x8F);
    set_idt_gate(4, (uint64_t)&overflow_eror, 0x28, 0x8F);
    set_idt_gate(5, (uint64_t)&bound_error, 0x28, 0x8F);
    set_idt_gate(8, (uint64_t)&double_fault, 0x28, 0x8F);
}

#endif