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

int atoi(const char *str) {
    int result = 0;

    if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X')) {
        str += 2;
    }

    while (*str != '\0') {
        char digit = *str;
        int value;

        if (digit >= '0' && digit <= '9') {
            value = digit - '0';
        } else if (digit >= 'a' && digit <= 'f') {
            value = 10 + digit - 'a';
        } else if (digit >= 'A' && digit <= 'F') {
            value = 10 + digit - 'A';
        } else {
            break;
        }

        result = result * 16 + value;
        ++str;
    }

    return result;
}

void printf(const char *format, ...) {
    unsigned fg = get_foreground();
    unsigned bg = get_background();

    va_list args;
    va_start(args, format);

    char cvalue[128];

    while (*format != '\0') {
        if (*format == '%') {
            format++;

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
                case 'x': {
                    int value = va_arg(args, int);
                    char buffer[128];
                    itoa(value, buffer, 16);
                    printstr("0x", get_foreground(), get_fontsize());
                    printstr(buffer, get_foreground(), get_fontsize());
                    break;
                }
                default:
                    putchar('%', get_foreground(), get_fontsize());
                    printstr(chtostr(*format), get_foreground(), get_fontsize());
            }
        } else if (*format == '#') {
            format++;
            if (*format == '{') {
                format++;
                int i = 0;

                while (*format && *format != '}') {
                    cvalue[i++] = *format;
                    format++;
                }

                cvalue[i] = '\0';
                set_foreground(atoi(cvalue));
            } else {
                putchar('#', get_foreground(), get_fontsize());
                printstr(chtostr(*format), get_foreground(), get_fontsize());
            }
        } else if (*format == '$') {
            format++;
            if (*format == '{') {
                format++;
                int i = 0;

                while (*format && *format != '}') {
                    cvalue[i++] = *format;
                    format++;
                }
                cvalue[i] = '\0';
                set_background(atoi(cvalue));
            } else {
                putchar('#', get_foreground(), get_fontsize());
                printstr(chtostr(*format), get_foreground(), get_fontsize());
            }
        } else {
            printstr(chtostr(*format), get_foreground(), get_fontsize());
        }

        format++;
    }

    va_end(args);

    set_foreground(fg);
    set_background(bg);
}