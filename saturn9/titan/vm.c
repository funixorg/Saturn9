#include <vm.h>
#include <executable.h>
#include <mem.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <syscallsv.h>

VMEnv *vm_env;
Executable *cexec;

Value *VM_read_reg(uint8_t opcode) {
    return vm_env->registers[opcode].val;
}

Function *VM_get_func(char *name) {
    for (unsigned _i=0; _i<cexec->text_size; _i++) {
        if (strcmp(cexec->text[_i]->name, name)) {
            return cexec->text[_i];
        }
    }
    return NULL;
}

Constant *VM_FindConst(char *name) {
    for (unsigned _i=0;_i<cexec->dcb_size;_i++) {
        if (strcmp(cexec->dcb[_i]->name, name)) {
            return cexec->dcb[_i];
        }
    }
    return NULL;
}

Variable *VM_FindVariable(char *name) {
    for (unsigned _i=0;_i<cexec->data_size;_i++) {
        if (strcmp(cexec->data[_i]->name, name)) {
            return cexec->data[_i];
        }
    }
    return NULL;
}

Value *ProcessValue(Value* val, bool ok_const) {
    if (val->type == &TYPE_REGISTER) {
        return vm_env->registers[val->u32_value].val;
    }
    else if (val->type == &TYPE_IDENTIFIER) {
        if (ok_const) {
            Constant *constf = VM_FindConst(val->str_value);
            if (constf) {
                return constf->value;
            }
        }
        Variable *varf = VM_FindVariable(val->str_value);
        if (varf) {
            return varf->value;
        }
        return NULL;
    }
    else {
        return val;
    }
}

void VM_Add(Operation* oper) {
    if (oper->target->type == &TYPE_REGISTER) {
        Value *sval = ProcessValue(oper->source, 1);
        if (sval) {
            vm_env->registers[oper->target->u32_value].val->u32_value += sval->u32_value;
        }
    }
    else if (oper->target->type == &TYPE_IDENTIFIER) {
        Variable *varf = VM_FindVariable(oper->target->str_value);
        if (varf) {
            Value *sval = ProcessValue(oper->source, 1);
            if (sval) {
                varf->value->u32_value += sval->u32_value;
            }
        }
    }

    else {}
}

void VM_Sub(Operation* oper) {
    if (oper->target->type == &TYPE_REGISTER) {
        Value *sval = ProcessValue(oper->source, 1);
        if (sval) {
            vm_env->registers[oper->target->u32_value].val->u32_value -= sval->u32_value;
        }
    }
    else if (oper->target->type == &TYPE_IDENTIFIER) {
        Variable *varf = VM_FindVariable(oper->target->str_value);
        if (varf) {
            Value *sval = ProcessValue(oper->source, 1);
            if (sval) {
                varf->value->u32_value -= sval->u32_value;
            }
        }
    }

    else {}
}

void VM_Mul(Operation* oper) {
    if (oper->target->type == &TYPE_REGISTER) {
        Value *sval = ProcessValue(oper->source, 1);
        if (sval) {
            vm_env->registers[oper->target->u32_value].val->u32_value *= sval->u32_value;
        }
    }
    else if (oper->target->type == &TYPE_IDENTIFIER) {
        Variable *varf = VM_FindVariable(oper->target->str_value);
        if (varf) {
            Value *sval = ProcessValue(oper->source, 1);
            if (sval) {
                varf->value->u32_value *= sval->u32_value;
            }
        }
    }

    else {}
}

void VM_Div(Operation* oper) {
    if (oper->target->type == &TYPE_REGISTER) {
        Value *sval = ProcessValue(oper->source, 1);
        if (sval) {
            vm_env->registers[oper->target->u32_value].val->u32_value /= sval->u32_value;
        }
    }
    else if (oper->target->type == &TYPE_IDENTIFIER) {
        Variable *varf = VM_FindVariable(oper->target->str_value);
        if (varf) {
            Value *sval = ProcessValue(oper->source, 1);
            if (sval) {
                varf->value->u32_value /= sval->u32_value;
            }
        }
    }

    else {}
}

void VM_Move(Operation* oper) {
    if (oper->target->type == &TYPE_REGISTER) {
        Value *sval = ProcessValue(oper->source, 1);
        if (sval) {
            vm_env->registers[oper->target->u32_value].val = sval;
        }
    }
    else if (oper->target->type == &TYPE_IDENTIFIER) {
        Variable *varf = VM_FindVariable(oper->target->str_value);
        if (varf) {
            Value *sval = ProcessValue(oper->source, 1);
            if (sval) {
                varf->value = sval;
            }
        }
    }

    else {}
}

void VM_Call(Operation *oper) {
    char *fn_name = oper->target->str_value;
    Function *fn_h = VM_get_func(fn_name);
    if (fn_h) {
        VM_run_func(fn_h, vm_env->parameters);
    }
}

void VM_Push(Operation *oper) {
    Value *sval = ProcessValue(oper->target, 1);
    if (sval) {
        VM_FnParam *param = memalloc(sizeof(VM_FnParam));
        param->val = sval;
        vm_env->parameters[vm_env->n_param] = param;
        vm_env->n_param++;
    }
}

void VM_Return() {
    for (unsigned _j=0; _j<vm_env->n_param;_j++) {
        vm_env->parameters[_j] = NULL;
    }
    vm_env->parameters = memalloc(sizeof(VM_FnParam)*8); // 8 Max Parameters
    vm_env->n_param=0;
}

void VM_Interrupt() {
    VM_run_syscall(vm_env->registers);
}

void VM_run_instruction(Operation* oper) {
    switch (oper->opcode) {
        case INS_MOV: {
            VM_Move(oper);
            break;
        }
        case INS_CALL: {
            VM_Call(oper);
            break;
        }
        case INS_PUSH: {
            VM_Push(oper);
            break;
        }
        case INS_RET: {
            VM_Return();
            break;
        }

        case INS_ADD: {
            VM_Add(oper);
            break;
        }
        case INS_SUB: {
            VM_Sub(oper);
            break;
        }
        case INS_MUL: {
            VM_Mul(oper);
            break;
        }
        case INS_DIV: {
            VM_Div(oper);
            break;
        }

        case INS_INT: {
            VM_Interrupt();
            break;
        }
    }
}

void VM_run_func(Function *func, VM_FnParam **params) {
    if (params) {
        for (unsigned _i=0;_i<vm_env->n_param; _i++) {
            char *pname = func->parameters[_i];
            Variable *pvar = VM_FindVariable(pname);
            if (pvar) {
                pvar->value = params[_i]->val;
            }
        }
    }

    for (unsigned _i=0; _i<func->nops; _i++) {
        VM_run_instruction(
            func->body[_i]
        );
    }
}

void VM_init_regs() {
    for (unsigned _i = 0; _i < 16; _i++) {
        Value init_val;
        init_val.type = &TYPE_INT32;
        init_val.u32_value = 0;
        init_val.str_value = NULL;
        vm_env->registers[_i].val = memalloc(sizeof(Value));
        *(vm_env->registers[_i].val) = init_val;
    }
}

unsigned VM_run_executable(Executable *exec, VM_FnParam **args) {
    vm_env = (VMEnv*)memalloc(sizeof(VMEnv));
    vm_env->registers = memalloc(sizeof(VM_Register)*16); // 16 registers
    vm_env->parameters = memalloc(sizeof(VM_FnParam)*8); // 8 Max Parameters
    vm_env->n_param = 0;
    VM_init_regs();
    cexec = exec;
    Function *main_fn = VM_get_func("main");
    if (!main_fn) {
        return 3; // Missing entry_point
    }
    if (args) {
        VM_run_func(main_fn, args);
    }
    else {
        VM_run_func(main_fn, NULL);
    }

    return 0;
}