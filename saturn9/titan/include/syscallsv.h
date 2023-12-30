#include <executable.h>
#include <opcodes.h>
#include <datatypes.h>
#include <vm.h>

#ifndef SYSCALLSV_H
#define SYSCALLSV_H

#define SYSC_CLEAR    0x90
#define SYSC_PRINT    0x91
#define SYSC_READLINE 0x92
#define SYSC_READFILE 0x93
#define SYSC_PUTPIXEL 0x94
#define SYSC_DRAWRECT 0x95

void VM_run_syscall(VM_Register *registers);

#endif