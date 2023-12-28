#include "idt.h"

#define IDT_ENTRIES 256

idt_entry_t idt[IDT_ENTRIES];
idt_pointer_t idt_p;

extern void load_idt(uint64_t);

void set_idt_gate(int num, uint64_t base, uint16_t sel, uint8_t flags) {
    idt[num].offset_low = (base & 0xFFFF);
    idt[num].offset_middle = (base >> 16) & 0xFFFF;
    idt[num].offset_high = (base >> 32) & 0xFFFFFFFF;
    idt[num].selector = sel;
    idt[num].ist = 0;
    idt[num].flags = flags;
    idt[num].zero = 0;
}

void idt_init() {
    idt_p.limit = sizeof(idt_entry_t) * IDT_ENTRIES - 1;
    idt_p.base = (uint64_t)&idt;

    for (int i = 0; i < IDT_ENTRIES; ++i) {
        set_idt_gate(i, 0, 0, 0);
    }

    load_idt((uint64_t)&idt_p);
}


idt_entry_t *get_idt() {
    return idt;
}