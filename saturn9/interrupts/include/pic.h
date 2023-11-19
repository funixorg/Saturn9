#include <stdint.h>
#include <portio.h>
#ifndef PIC_H
#define PIC_H

#define PIC_1_CMD  0x20
#define PIC_1_DATA 0x21

#define PIC_2_CMD  0xA0
#define PIC_2_DATA 0xA1

#define NEW_IRQ_0  0x20
#define NEW_IRQ_8  0x28

#define PIC_EOI    0x20


void send_pic_eoi(uint8_t irq) {
    if (irq>=8) outb(PIC_2_CMD, PIC_EOI);

    outb(PIC_1_CMD, PIC_EOI);
}

void disable_pic(void) {
    outb(PIC_2_DATA, 0xFF);
    outb(PIC_1_DATA, 0xFF);
}

void set_irq_mask(uint8_t irq) {
    uint16_t port;
    uint8_t value;

    if (irq<8) port = PIC_1_DATA;
    else {
        irq -= 8;
        port = PIC_2_DATA;
    }

    value = inb(port) | (1 << irq);
    outb(port, value);
}

void clear_irq_mask(uint8_t irq) {
    uint16_t port;
    uint8_t value;

    if (irq<8) port = PIC_1_DATA;
    else {
        irq -= 8;
        port = PIC_2_DATA;
    }

    value = inb(port) & ~(1 << irq);
    outb(port, value);
}

void remap_pic(void) {
    uint8_t pic_1_mask, pic_2_mask;

    pic_1_mask = inb(PIC_1_DATA);
    pic_2_mask = inb(PIC_2_DATA);

    outb(PIC_1_CMD, 0x11);
    io_wait();
    outb(PIC_2_CMD, 0x11);
    io_wait();

    outb(PIC_1_DATA, NEW_IRQ_0);
    io_wait();
    outb(PIC_2_DATA, NEW_IRQ_8);
    io_wait();

    outb(PIC_1_DATA, 0x4);
    io_wait();
    outb(PIC_2_DATA, 0x2);
    io_wait();

    outb(PIC_1_DATA, 0x1);
    io_wait();
    outb(PIC_2_DATA, 0x1);
    io_wait();

    outb(PIC_1_DATA, pic_1_mask);
    outb(PIC_2_DATA, pic_2_mask);
}

#endif