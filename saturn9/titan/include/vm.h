#include <stdint.h>
#include <datatypes.h>
#include <opcodes.h>
#include <executable.h>


#ifndef VM_H
#define VM_H

typedef struct VM_Register {
    Value *val;
} VM_Register;

typedef struct VM_FnParam {
    Value *val;
} VM_FnParam;

typedef struct VMEnv {
    VM_Register *registers;
    VM_FnParam  **parameters;
    uint8_t n_param;
} VMEnv;

Value *VM_read_reg(uint8_t opcode);
void VM_run_func(Function *func, VM_FnParam **params);
unsigned VM_run_executable(Executable *exec, VM_FnParam **params);

#endif