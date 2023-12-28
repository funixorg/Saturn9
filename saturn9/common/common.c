#include <common.h>
#include <stdint.h>
#include <stdio.h>

void hcf() {
    asm ("cli");
    for (;;) {
        asm ("hlt");
    }
}

void cpuid(uint32_t eax, uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d) {
    asm volatile("cpuid" : "=a"(*a), "=b"(*b), "=c"(*c), "=d"(*d) : "a"(eax));
}

uint64_t rax_val, rbx_val, rcx_val, rdx_val;

void print_registers() {
    asm volatile (
        "movq %%rax, %0\n\t"
        "movq %%rbx, %1\n\t"
        "movq %%rcx, %2\n\t"
        "movq %%rdx, %3\n\t"
        : "=r"(rax_val), "=r"(rbx_val), "=r"(rcx_val), "=r"(rdx_val)
        :
        : // No clobbered registers
    );

    // Print register values
    printf("RAX: %x\n", rax_val);
    printf("RBX: %x\n", rbx_val);
    printf("RCX: %x\n", rcx_val);
    printf("RDX: %x\n", rdx_val);
}



int sys_init_fpu() {
    // get if fpu is present
    uint32_t eax, ebx, ecx, edx;
    cpuid(1, &eax, &ebx, &ecx, &edx);
    if (!(edx & (1 << 24)))
        return 1;
    // enable fpu
    asm volatile("fninit");
    asm volatile("fwait");
    asm volatile("clts");
    asm volatile("mov %cr0, %rax");
    asm volatile("and $0x9FFFFFFF, %eax");
    asm volatile("mov %rax, %cr0");
    asm volatile("mov %cr4, %rax");
    asm volatile("or $0x600, %rax");
    asm volatile("mov %rax, %cr4");
    return 0;
}