#include <stdint.h>
#include <stdlib.h>

#ifndef IDT_H
#define IDT_H

#define TRAP_GATE_FLAGS 0xF
#define INT_GATE_FLAGS 0xE
#define CALL_GATE_FLAGS 0xC


// IDT Entry
typedef struct {
    uint16_t offset_low;        // Offset bits 0-15
    uint16_t segment_selector;  // Code segment selector
    uint8_t ist;                // Interrupt Stack Table offset (bits 0-2)
    uint8_t attributes;         // Type and attributes
    uint16_t offset_middle;     // Offset bits 16-31
    uint32_t offset_high;       // Offset bits 32-63
    uint32_t reserved;          // Reserved
} __attribute__((packed)) idt_entry_64_t;


// IDTR layout
typedef struct {
    uint16_t limit;  // Limit (size) of the IDT
    uint64_t base;   // Base address of the IDT
} __attribute__((packed)) idtr_64_t;


// Pushed Registers for IRS
typedef struct {
    uint64_t ds;
    uint64_t rdi, rsi, rbp, rsp, rbx, rdx, rcx, rax;
    uint64_t intNo, errCode;
    uint64_t rip, cs, eflags, useresp, ss;
} registers;


// Interrupt frame for handlers
typedef struct {
    uint64_t rip;    // Instruction pointer
    uint64_t cs;     // Code segment selector
    uint64_t rflags; // Flags register
    uint64_t rsp;    // Stack pointer
    uint64_t ss;     // Stack segment selector
} __attribute__((packed)) int_frame_64_t;

void set_idt_descriptor_64(uint8_t entry_number, void* isr, uint8_t flags);

void init_idt_64(void);
#endif