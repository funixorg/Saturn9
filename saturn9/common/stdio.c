#include <stdio.h>
#include <stdlib.h>
#include <serial.h>
#include <stdarg.h>
#include <graphics.h>
#include <keyboard.h>

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

void printf_serial(const char *format, ...) {
    va_list args;
    va_start(args, format);

    while (*format != '\0') {
        if (*format == '%') {
            format++;

            switch (*format) {
                case 'd': {
                    int value = va_arg(args, int);
                    char buffer[128];
                    itoa(value, buffer, 10);
                    write_serial(buffer);
                    break;
                }
                case 's': {
                    const char *str = va_arg(args, const char *);
                    write_serial(str);
                    break;
                }
                case 'x': {
                    int value = va_arg(args, int);
                    char buffer[128];
                    itoa(value, buffer, 16);
                    write_serial("0x");
                    write_serial(buffer);
                    break;
                }
                default:
                    write_serial_ch('%');
                    write_serial_ch(*format);
            }
        } else {
            write_serial_ch(*format);
        }

        format++;
    }

    va_end(args);
}

char *readline(char *prompt) {
  printf("%s", prompt);
  char *buffer = read_keyboard();
  return buffer;
}

void clear_screen() {
  draw_screen(get_background());
  set_y_offset(DEFAULT_Y);
}
