#include <syscallsv.h>
#include <executable.h>
#include <opcodes.h>
#include <datatypes.h>
#include <vm.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <mem.h>
#include <filesystem.h>
#include <graphics.h>

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

void VM_sysc_readfile() {
    Value *rbxval = VM_read_reg(REG_RBX);
    Value *rsival = VM_read_reg(REG_RSI);
    if (rbxval->str_value) {
        printf_serial("path: %s\n", rbxval->str_value);
        rsival->type = &TYPE_STRING;
        rsival->str_value = VFS_read_path(rbxval->str_value);
        if (!rsival->str_value) {
            rsival->str_value = "#{0xff0000}`#{0xb14e4e}%s#{0xff0000}` file not found!";
        }
    }
    else {
        rsival->str_value = "\\b";
    }
}

void VM_sysc_putpixel() {
    Value *r8val = VM_read_reg(REG_R8);
    Value *r9val = VM_read_reg(REG_R9);
    Value *rbpval = VM_read_reg(REG_RBP);
    if (r8val->u32_value && r9val->u32_value && rbpval->u32_value) {
        GP_draw_pixel(r8val->u32_value, r9val->u32_value, rbpval->u32_value);
    }
}

void VM_sysc_drawrect() {
    Value *r8val = VM_read_reg(REG_R8); // x
    Value *r9val = VM_read_reg(REG_R9); // y
    Value *r10val = VM_read_reg(REG_R10); // width
    Value *r11val = VM_read_reg(REG_R11); // height
    Value *rbpval = VM_read_reg(REG_RBP); // color
    if (r8val->u32_value && r9val->u32_value && r10val->u32_value && r11val->u32_value &&rbpval->u32_value) {
        GP_draw_rectangle(r8val->u32_value, r9val->u32_value,r10val->u32_value, r11val->u32_value, rbpval->u32_value);
    }
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
        case SYSC_READFILE: {
            VM_sysc_readfile();
            break;
        }
        case SYSC_PUTPIXEL: {
            VM_sysc_putpixel();
            break;
        }
        case SYSC_DRAWRECT: {
            VM_sysc_drawrect();
            break;
        }
    }
}