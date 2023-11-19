#include <stdlib.h>
#include <stdio.h>

#ifndef SYSCALLS_H
#define SYSCALLS_H

#define MAX_SYSCALLS 2

void syscall_test0(void) {
    printf("SysC_0\n");
}
void syscall_test1(void) {
    printf("SysC_1\n");
}

void *syscalls[MAX_SYSCALLS] = {
    syscall_test0,
    syscall_test1
};

__attribute__((naked)) void syscall_dispatcher(void) {
    __asm__ __volatile__(
    ".intel_syntax noprefix\n"
    
    ".equ MAX_SYSCALLS, 2\n"
    "cmp rax, MAX_SYSCALLS-1\n"
    "ja invalid_syscall\n"

    "push rax\n"
    "push gs\n"
    "push fs\n"
    "push rbp\n"
    "push rdi\n"
    "push rsi\n"
    "push rdx\n"
    "push rcx\n"
    "push rbx\n"
    "push rsp\n"
    "mov rdi, rax\n"            // Move syscall number to rdi for indexing
    "mov rax, [syscalls + rdi * 8]\n" // Load the address of the syscall function
    "call rax\n"                // Call the syscall function
    "add rsp, 8\n"              // Adjust the stack after the call
    "pop rbx\n"
    "pop rcx\n"
    "pop rdx\n"
    "pop rsi\n"
    "pop rdi\n"
    "pop rbp\n"
    "pop fs\n"
    "pop gs\n"
    "add esp, 4\n"
    "iretq\n"

    "invalid_syscall:\n"
    "iretq\n"
    
    ".att_syntax"
    );
}



#endif