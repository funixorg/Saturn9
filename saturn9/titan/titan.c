#include <titan.h>
#include <stdio.h>
#include <stdint.h>

void raw_exec(const char* machine_code) {
    int_fn function = (int_fn)machine_code;
    unsigned ret = function();
    printf_serial("F_ret: %d\n", ret);
}

void test_titan() {
    const char machineCode[] = {
        0x48, 0x83, 0xEC, 0x08,             // sub rsp, 8
        0xB8, 0x02, 0x00, 0x00, 0x00,       // mov eax, 2
        0x48, 0x83, 0xC4, 0x08,             // add rsp, 8
        0xC3                                // ret
    };

    raw_exec(machineCode);

    void (*printf_s_p)() = &printf_serial;
    uintptr_t address = (uintptr_t)printf_s_p;
}