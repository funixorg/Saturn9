#include <idt.h>
#include <stdlib.h>

#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

// (0-31 in IDT)

__attribute__ ((interrupt)) void div_by_0_handler(int_frame_64_t *frame) {
    //printf("#{0xff0000} DIVDE BY 0 ERROR - EXCEPTION HANDLED\n");
    frame->rip++;
}

#endif