#include <stdint.h>

#ifndef OPCODES_H
#define OPCODES_H


#define TITAN_SIGNATURE 0x18A
#define FUNCTION_SIGNATURE 0xF0

#define INS_MOV  0x00
#define INS_CALL 0x01
#define INS_PUSH 0x02
#define INS_RET  0x03
#define INS_LEA  0x04

#define INS_ADD  0x05
#define INS_SUB  0x06
#define INS_MUL  0x07
#define INS_DIV  0x08

#define INS_INT  0x09


#define REG_RAX 0x00
#define REG_RCX 0x01
#define REG_RDX 0x02

#define REG_RBX 0x03
#define REG_RSP 0x04
#define REG_RBP 0x05
#define REG_RSI 0x06
#define REG_RDI 0x07

#define REG_R8  0x08
#define REG_R9  0x09
#define REG_R10 0x0A
#define REG_R11 0x0B
#define REG_R12 0x0C
#define REG_R13 0x0D
#define REG_R14 0x0E
#define REG_R15 0x0F

#endif