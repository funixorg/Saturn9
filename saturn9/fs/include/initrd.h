#ifndef INITRD_H
#define INITRD_H

#include <stdint.h>
#include "filesystem.h"

#define LIMINE_INTERNAL_MODULE_REQUIRED (1 << 0)

void initramdisk();

#endif