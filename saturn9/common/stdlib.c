#include <stdlib.h>
#include <graphics.h>
#include <stdarg.h>
#include <mem.h>


void reverse(char str[], int length) {
    int start = 0;
    int end = length - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

void itoa(int num, char str[], int base) {
    int i = 0;
    int is_negative = 0;
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }
    if (num < 0 && base != 10) {
        is_negative = 1;
        num = -num;
    }
    while (num != 0) {
        int remainder = num % base;
        str[i++] = (remainder > 9) ? (remainder - 10) + 'a' : remainder + '0';
        num = num / base;
    }
    if (is_negative && base != 10) {
        str[i++] = '-';
    }
    str[i] = '\0';
    reverse(str, i);
}

char* chtostr(char c) {
    char* str = (char*)memalloc(2);
    if (str == NULL) {
        return NULL;
    }

    str[0] = c;
    str[1] = '\0';

    return str;
}

void printf(const char *format, ...) {
    va_list args;
    va_start(args, format);

    while (*format != '\0') {
        if (*format == '%') {
            format++; // Move past '%'

            switch (*format) {
                case 'd': {
                    int value = va_arg(args, int);
                    char buffer[128];
                    itoa(value, buffer, 10);
                    printstr(buffer, get_foreground(), get_fontsize());
                    break;
                }
                case 's': {
                    const char *str = va_arg(args, const char *);
                    printstr(str, get_foreground(), get_fontsize());
                    break;
                }
                default:
                    putchar('%', get_foreground(), get_fontsize());
                    printstr(chtostr(*format), get_foreground(), get_fontsize());
            }
        } else {
            printstr(chtostr(*format), get_foreground(), get_fontsize());
        }

        format++;
    }

    va_end(args);
}