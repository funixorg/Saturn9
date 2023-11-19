#include <stdint.h>
#include <stack.h>

uint8_t stack[STACK_SIZE];

void stack_init() {
    __asm__ __volatile__ ("mov %%rsp,%0" :: "r" (stack + STACK_SIZE));
}