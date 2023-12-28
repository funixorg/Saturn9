#include <stdint.h>

#ifndef DATATYPES_H
#define DATATYPES_H

typedef struct Pointer {
    uint32_t address;
} Pointer;

typedef struct Var {
    char *name;
} Var;

typedef struct DataType {
    unsigned size;
    uint8_t id;
} DataType;

typedef struct Value {
    DataType *type;
    uint32_t u32_value;
    char *str_value;
} Value;


extern DataType TYPE_INT32;
extern DataType TYPE_INT8;
extern DataType TYPE_POINTER;
extern DataType TYPE_REGISTER;
extern DataType TYPE_IDENTIFIER;
extern DataType TYPE_STRING;

#endif