#include <stddef.h>
#include <stdint.h>

#ifndef STDLIB_H
#define STDLIB_H

#define MAX_SUBSTRINGS 50
#define MAX_SUBSTRING_LENGTH 512

int atoi(const char *str, unsigned base);
void itoa(int num, char str[], int base);
uint32_t rgbtohex(unsigned int R, unsigned int G, unsigned int B);
void reverse(char str[], int length);
size_t strlen(const char *str);
char* strdup(const char* str);
char **strsplit(char *s, const char delim);
char** tok_split(char* a_str, const char a_delim);
void to_upper(char *lower, const char *str);
void to_lower(char *upper, const char *str);
int strcmp(const char* str1, const char* str2);
char *strncat (char *s1, const char *s2, size_t n);
char* strcpy(char* dest, const char* src);
char* unix_to_time(uint32_t unix_time);
char* formatBytes(uint64_t bytes);
char *format(const char *format, ...);

#endif
