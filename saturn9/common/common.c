#include <common.h>

void hcf() {
    asm ("cli");
    for (;;) {
        asm ("hlt");
    }
}


