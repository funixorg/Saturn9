#include <mem.h>

static char memory_pool[MEMORY_POOL_SIZE];
static char *memory_pool_ptr = memory_pool;

void *memcpy(void *dest, const void *src, size_t n) {
    uint8_t *pdest = (uint8_t *)dest;
    const uint8_t *psrc = (const uint8_t *)src;

    for (size_t i = 0; i < n; i++) {
        pdest[i] = psrc[i];
    }

    return dest;
}


void *memset(void *s, int c, size_t n) {
    uint8_t *p = (uint8_t *)s;

    for (size_t i = 0; i < n; i++) {
        p[i] = (uint8_t)c;
    }

    return s;
}


void *memmove(void *dest, const void *src, size_t n) {
    uint8_t *pdest = (uint8_t *)dest;
    const uint8_t *psrc = (const uint8_t *)src;

    if (src > dest) {
        for (size_t i = 0; i < n; i++) {
            pdest[i] = psrc[i];
        }
    } else if (src < dest) {
        for (size_t i = n; i > 0; i--) {
            pdest[i-1] = psrc[i-1];
        }
    }

    return dest;
}


int memcmp(const void *s1, const void *s2, size_t n) {
    const uint8_t *p1 = (const uint8_t *)s1;
    const uint8_t *p2 = (const uint8_t *)s2;

    for (size_t i = 0; i < n; i++) {
        if (p1[i] != p2[i]) {
            return p1[i] < p2[i] ? -1 : 1;
        }
    }

    return 0;
}

void *memalloc(size_t size) {
    if (memory_pool_ptr + size <= memory_pool + MEMORY_POOL_SIZE) {
        void *allocation = memory_pool_ptr;
        memory_pool_ptr += size;
        return allocation;
    } else {
        return NULL;
    }
}

void initialize_memory_pool() {
    memory_pool_ptr = memory_pool;
}

void free(void *ptr) {
    uintptr_t pool_start = (uintptr_t)memory_pool;
    uintptr_t pool_end = pool_start + MEMORY_POOL_SIZE;
    uintptr_t ptr_value = (uintptr_t)ptr;

    if (ptr_value >= pool_start && ptr_value < pool_end) {
        memory_pool_ptr = ptr;
    }
}