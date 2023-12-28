#include <stdio.h>
#include <stdlib.h>
#include <serial.h>
#include <stdarg.h>
#include <graphics.h>
#include <terminal.h>
#include <keyboard.h>
#include <mem.h>
#include <pit.h>

void printf(const char *format, ...) {
    va_list args;
    va_start(args, format);

    char cvalue[128];

    while (*format != '\0') {
        if (*format == '%') {
            format++;

            switch (*format) {
                case 'd': {
                    int value = va_arg(args, int);
                    char buffer[16];
                    itoa(value, buffer, 10);
                    TERM_printstr(buffer, TERM_get_foreground(), TERM_get_fontsize());
                    break;
                }
                case 's': {
                    const char *str = va_arg(args, const char *);
                    TERM_printstr(str, TERM_get_foreground(), TERM_get_fontsize());
                    break;
                }
                case 'x': {
                    int value = va_arg(args, int);
                    char buffer[16];
                    itoa(value, buffer, 16);
                    TERM_printstr("0x", TERM_get_foreground(), TERM_get_fontsize());
                    TERM_printstr(buffer, TERM_get_foreground(), TERM_get_fontsize());
                    break;
                }
                case 'c': {
                    char value = va_arg(args, int);
                    char buffer[2];
                    buffer[0]=value;
                    buffer[1]='\0';
                    TERM_printstr(buffer, TERM_get_foreground(), TERM_get_fontsize());
                    break;
                }
                default:
                    TERM_putchar('%', TERM_get_foreground(), TERM_get_fontsize());
                    TERM_putchar(*format, TERM_get_foreground(), TERM_get_fontsize());

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
                TERM_set_foreground(atoi(cvalue, 16));
            } else {
                TERM_putchar('#', TERM_get_foreground(), TERM_get_fontsize());
                TERM_putchar(*format, TERM_get_foreground(), TERM_get_fontsize());
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
                TERM_set_background(atoi(cvalue, 16));
            } else {
                TERM_putchar('$', TERM_get_foreground(), TERM_get_fontsize());
                TERM_putchar(*format, TERM_get_foreground(), TERM_get_fontsize());
            }
        } 
        else if (*format == '\\') {
            format++;
            if (*format == '@') {
                format++;
                if (*format == 'R') {
                    format++;
                    if (*format == 'E') {
                        format++;
                        if (*format == 'S') {
                            TERM_set_foreground(TERM_get_default_fg());
                            TERM_set_background(TERM_get_default_bg());
                        }
                        else {
                            TERM_putchar('E', TERM_get_foreground(), TERM_get_fontsize());
                            TERM_putchar(*format, TERM_get_foreground(), TERM_get_fontsize());
                        }
                    }
                    else {
                        TERM_putchar('R', TERM_get_foreground(), TERM_get_fontsize());
                        TERM_putchar(*format, TERM_get_foreground(), TERM_get_fontsize());
                    }
                } 
                else {
                    TERM_putchar('@', TERM_get_foreground(), TERM_get_fontsize());
                    TERM_putchar(*format, TERM_get_foreground(), TERM_get_fontsize());
                }
            }
            else {
                TERM_putchar('\\', TERM_get_foreground(), TERM_get_fontsize());
                TERM_putchar(*format, TERM_get_foreground(), TERM_get_fontsize());
            }
        }
        else {
            TERM_putchar(*format, TERM_get_foreground(), TERM_get_fontsize());
        }

        format++;
    }

    va_end(args);
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
                    char buffer[16];
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
                    char buffer[16];
                    itoa(value, buffer, 16);
                    write_serial("0x");
                    write_serial(buffer);
                    break;
                }
                case 'c': {
                    char value = va_arg(args, int);
                    char buffer[2];
                    buffer[0]=value;
                    buffer[1]='\0';
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


char *readline() {
    clearbuffer();
    while (readkey() != '\n') {}
    char *buffer=readbuffer();
    setkey('\0');
    return buffer;
}

void clear_screen() {
  GP_draw_screen(TERM_get_background());
  TERM_set_ypos(DEFAULT_Y);
}
