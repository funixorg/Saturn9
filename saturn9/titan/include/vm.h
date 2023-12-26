#include <stdint.h>
#include <datatypes.h>
#include <opcodes.h>
#include <executable.h>


#ifndef VM_H
#define VM_H

typedef struct VM_Register {
    Value *val;
} VM_Register;

typedef struct VMEnv {
    VM_Register *registers;
} VMEnv;

unsigned VM_run_executable(Executable *exec);

#endif