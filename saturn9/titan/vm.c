#include <vm.h>
#include <executable.h>
#include <mem.h>
#include <stdlib.h>
#include <stdio.h>

VMEnv *vm_env;
Executable *cexec;

Function *VM_get_func(char *name) {
    for (unsigned _i=0; _i<cexec->text_size; _i++) {
        if (strcmp(cexec->text[_i].name, name)) {
            return &(cexec->text[_i]);
        }
    }
    return NULL;
}

void VM_run_instruction(Operation* oper) {
    printf_serial("X: %d\n", oper->opcode);
}

void VM_run_func(Function *func) {
    for (unsigned _i=0; _i<func->nops; _i++) {
        VM_run_instruction(
            func->body[_i]
        );
    }
}

unsigned VM_run_executable(Executable *exec) {
    vm_env = (VMEnv*)memalloc(sizeof(VMEnv));
    cexec = exec;
    Function *main_fn = VM_get_func("main");
    if (!main_fn) {
        return 3; // Missing entry_point
    }
    VM_run_func(main_fn);

    return 0;
}