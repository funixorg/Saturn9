#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifndef MEM_H
#define MEM_H

#define MEMORY_POOL_SIZE 12288

void *memcpy(void *dest, const void *src, size_t n);
void *memset(void *s, int c, size_t n);
void *memmove(void *dest, const void *src, size_t n);
int memcmp(const void *s1, const void *s2, size_t n);
void *memalloc(size_t size);
void free(void *ptr);

#endif