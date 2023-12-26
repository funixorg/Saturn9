#include <idt.h>
#include <stdio.h>
#include <pit.h>
#include <pic.h>

#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

// (0-31 in IDT)

__attribute__((interrupt)) void division_by_zero_error(void*) {
    printf("Division by zero!\n");
}

void init_os_interupts() {
    set_idt_gate(0, (uint64_t)&division_by_zero_error, 0x28, 0x8E);
}

#endif