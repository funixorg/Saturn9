#include <idt.h>
#include <stdint.h>
#include <stdio.h>

// Default exception handler without errcode
__attribute__ ((interrupt)) void default_excp_handler(int_frame_64_t *int_frame_64) {
    printf("DEFAULT EXCEPTION HANDLER - ERROR CODE: N/A\n");
}

// Default exception handler with errcode
__attribute__ ((interrupt)) void default_excp_handler_err_code(int_frame_64_t *int_frame_64, unsigned long int error_code) {
    printf("DEFAULT EXCEPTION HANDLER - ERROR CODE: %d\n", error_code);
}

// Default interrupt handler
__attribute__ ((interrupt)) void default_int_handler(int_frame_64_t *int_frame_64, unsigned long int error_code) {
    printf("DEFAULT INTERRUPT HANDLER\n");
}

idt_entry_64_t idt64[256]; // IDT
idtr_64_t idtr64;          // IDT instance


void set_idt_descriptor_64(uint8_t entry_number, void* isr, uint8_t flags) {
    idt_entry_64_t *descriptor = &idt64[entry_number];
    descriptor->offset_low = (uint16_t)((uint64_t)isr & 0xFFFF);
    descriptor->segment_selector = (uint16_t)(((uint64_t)isr >> 16) & 0xFFFF);
    descriptor->ist = 0;  // You may adjust this based on your needs
    descriptor->attributes = flags;
    descriptor->offset_middle = (uint16_t)(((uint64_t)isr >> 32) & 0xFFFF);
    descriptor->offset_high = (uint32_t)(((uint64_t)isr >> 48) & 0xFFFFFFFF);
    descriptor->reserved = 0;
}

void init_idt_64(void) {
    idtr64.limit = (uint16_t)(8*255); // 256 descriptors 8 bytes each
    idtr64.base = (uint64_t)&idt64[0];

    // Exception handlers (0-31)
    for (uint8_t entry=0; entry < 32;entry++) {
        if (entry == 8 || entry == 10 || entry == 11 || entry == 12 ||
            entry == 13 || entry == 14 || entry == 17 || entry == 21) {
                // error_code
                set_idt_descriptor_64(entry, default_excp_handler_err_code, TRAP_GATE_FLAGS);
        } else {
            // No error_code
            set_idt_descriptor_64(entry, default_excp_handler, TRAP_GATE_FLAGS);
        }
    }


    // Interrupts (32-255)
    for (uint8_t entry=32; entry < 255;entry++) {
        set_idt_descriptor_64(entry, default_int_handler, INT_GATE_FLAGS);
    }

    __asm__ __volatile__ ("lidt %0" : : "memory"(idtr64)); // Load IDT into IDT Register
    __asm__ __volatile__ ("sti");
}