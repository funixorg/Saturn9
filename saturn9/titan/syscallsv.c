#include <syscallsv.h>
#include <executable.h>
#include <opcodes.h>
#include <datatypes.h>
#include <vm.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <mem.h>

void VM_sysc_clear() {
    clear_screen();
}

void VM_sysc_print() {
    // RBX = String Value
    Value *rbxval = VM_read_reg(REG_RBX);
    if (rbxval->str_value) {
        printf(rbxval->str_value);
    }
    else {
        char *buffer=memalloc(sizeof(char)*8);
        itoa(rbxval->u32_value, buffer, 10);
        printf(buffer);
    }
}

void VM_sysc_readline() {
    Value *rsival = VM_read_reg(REG_RSI);
    rsival->type = &TYPE_STRING;
    rsival->str_value = readline();
}

void VM_run_syscall(VM_Register *registers) {
    uint32_t raxval = VM_read_reg(REG_RAX)->u32_value;
    switch (raxval) {
        case SYSC_CLEAR: {
            VM_sysc_clear();
            break;
        }
        case SYSC_PRINT: {
            VM_sysc_print();
            break;
        }
        case SYSC_READLINE: {
            VM_sysc_readline();
            break;
        }
    }
}