#include <serial.h>
#include <stdlib.h>
#include <stdarg.h>

int serial_init() {
    outb(PORT + 1, 0x00);   
    outb(PORT + 3, 0x80);   
    outb(PORT + 0, 0x03); 
    outb(PORT + 1, 0x00);  
    outb(PORT + 3, 0x03); 
    outb(PORT + 2, 0xC7);   
    outb(PORT + 4, 0x0B);  
    outb(PORT + 4, 0x1E);   
    outb(PORT + 0, 0xAE);    
    if(inb(PORT + 0) != 0xAE) {
        return 1;
    }

    // If serial is not faulty set it in normal operation mode
    // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
    outb(PORT + 4, 0x0F);
    return 0;
}

int is_transmit_empty() {
    return inb(PORT + 5) & 0x20;
}

void write_serial_ch(char ch) {
    while (is_transmit_empty() == 0);

    outb(PORT, ch);
}

void write_serial(const char* str) {
    while (*str) {
        write_serial_ch(*str);
        str++;
    }
    write_serial_ch('\0');
}