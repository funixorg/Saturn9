#include <portio.h>

void outb(uint16_t port, uint8_t value) {
    __asm__ __volatile__ ("outb %0, %1" : : "a"(value), "Nd"(port));
}

uint8_t inb(uint16_t port) {
    uint8_t ret_val=0;
    __asm__ __volatile__ ("inb %1, %0" : "=a"(ret_val) : "Nd"(port));
    return ret_val;
}

void io_wait(void) {
    __asm__ __volatile__ ("outb %%al, $0x80" :: "a"(0));
}