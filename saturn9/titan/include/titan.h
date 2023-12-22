

#ifndef TITAN_H
#define TITAN_H

typedef int (*int_fn)();
typedef int (*char_fn)();
typedef int (*void_fn)();

void raw_exec(const char* machineCode);
void test_titan();
#endif