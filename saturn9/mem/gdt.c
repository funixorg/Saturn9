#include <gdt.h>
#include <stdint.h>

struct gdt_entry gdt_entries[3];

// GDT pointer
struct gdt_ptr gdt_ptr;

// Function to initialize GDT entry
void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity) {
    gdt_entries[num].base_low = (base & 0xFFFF);
    gdt_entries[num].base_middle = (base >> 16) & 0xFF;
    gdt_entries[num].base_high = (base >> 24) & 0xFF;

    gdt_entries[num].limit_low = (limit & 0xFFFF);
    gdt_entries[num].granularity = ((limit >> 16) & 0x0F) | (granularity & 0xF0);

    gdt_entries[num].granularity |= 0xC0; // Set D/B bit to 1 for 32-bit code
    gdt_entries[num].access = access;
}

void gdt_init() {
    // Set up GDT entries
    gdt_set_gate(0, 0, 0, 0, 0); // Null segment
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment

    // Load the GDT
    gdt_ptr.limit = sizeof(gdt_entries) - 1;
    gdt_ptr.base = (uint32_t)&gdt_entries;

    // Load GDT into the GDTR register
    __asm__ __volatile__("lgdt %0" : : "m"(gdt_ptr));
}