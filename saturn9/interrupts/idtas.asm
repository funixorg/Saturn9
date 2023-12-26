section .text
global load_idt

load_idt:
    mov rax, rdi
    lidt [rax]
    sti
    ret
