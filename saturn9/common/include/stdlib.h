#include <stddef.h>
#include <stdint.h>

#ifndef STDLIB_H
#define STDLIB_H

#define MAX_SUBSTRINGS 50
#define MAX_SUBSTRING_LENGTH 512

int atoi(const char *str);
char* chtostr(char c);
void itoa(int num, char str[], int base);
void reverse(char str[], int length);
size_t strlen(const char *str);
char** str_split(char* a_str, const char a_delim);
void to_upper(char *lower, const char *str);
void to_lower(char *upper, const char *str);
int strcmp(const char* str1, const char* str2);
char* cstrncat(char* destination, const char* source);
char* unix_to_time(uint32_t unix_time);
char* formatBytes(uint64_t bytes);

#endif
