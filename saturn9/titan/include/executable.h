#include <datatypes.h>
#include <opcodes.h>
#include <stdint.h>

#ifndef EXECUTABLE_H
#define EXECUTABLE_H

typedef struct Constant {
    char *name;
    Value *value;
    uint32_t address;
} Constant;

typedef struct Variable {
    char *name;
    Value *value;
    uint32_t address;
} Variable;

typedef struct Operation {
    uint8_t opcode;
    Value *target;
    Value *source;
} Operation;

typedef struct Function {
    char *name;
    char **parameters;
    Operation **body; 
    uint32_t nops;
    uint32_t nparms;
    uint32_t address;
} Function;

typedef struct Executable {
    Constant *dcb;
    Variable *data;
    Function *text;
    uint32_t dcb_size;
    uint32_t data_size;
    uint32_t text_size;
    uint32_t address;
} Executable;

#endif