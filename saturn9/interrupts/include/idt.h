#ifndef IDT_H
#define IDT_H

#include <stdint.h>

typedef struct {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t ist;
    uint8_t flags;
    uint16_t offset_middle;
    uint32_t offset_high;
    uint32_t zero;
} __attribute__((packed)) idt_entry_t;

typedef struct {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed)) idt_pointer_t;

void load_idt(uint64_t);
void trigger_interupt(uint64_t a);
void set_idt_gate(int num, uint64_t base, uint16_t sel, uint8_t flags);
void idt_init();
idt_entry_t *get_idt();

#endif /* IDT_H */
