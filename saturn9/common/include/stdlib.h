#include <stddef.h>

#ifndef STDLIB_H
#define STDLIB_H

int atoi(const char *str);
char* chtostr(char c);
void itoa(int num, char str[], int base);
void reverse(char str[], int length);
size_t strlen(const char *str);

#endif