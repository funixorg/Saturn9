#ifndef STDIO_H
#define STDIO_H

#define MAX_BUFFER_SIZE 512

void printf(const char *format, ...);
char *readstr();
void printf_serial(const char *format, ...);
char *readline();

#endif