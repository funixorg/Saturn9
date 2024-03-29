#include <stdlib.h>
#include <graphics.h>
#include <stdarg.h>
#include <stddef.h>
#include <mem.h>
#include <string.h>


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

uint32_t rgbtohex(unsigned int R, unsigned int G, unsigned int B) {
    return (R << 16) | (G << 8) | B;
}

int atoi(const char *str, unsigned base) {
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

        result = result * base + value;
        ++str;
    }

    return result;
}

size_t strlen(const char *str) {
    size_t i;

    for (i = 0; str[i]; i++);
    return i;
}


char* strtok(char* str, const char* delim) {
    static char* saved_str = NULL;
    if (str != NULL) {
        saved_str = str;
    } else if (saved_str == NULL) {
        return NULL;  
    }
    char* token_start = saved_str;
    while (*saved_str != '\0') {
        size_t delim_index = 0;
        while (delim[delim_index] != '\0') {
            if (*saved_str == delim[delim_index]) {
                *saved_str = '\0';  
                saved_str++;       
                return token_start;
            }
            delim_index++;
        }
        saved_str++;
    }

    if (token_start == saved_str) {
        return NULL; 
    }
    return token_start;
}

char** tok_split(char* a_str, const char a_delim) {
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }
    count += last_comma < (a_str + strlen(a_str) - 1);
    count++;
    result = memalloc(sizeof(char*) * count);
    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            //assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        //assert(idx == count - 1);
        *(result + idx) = 0;
    }
    return result;
}

char **strsplit(char *s, const char delim) {
    char **tokens = memalloc(sizeof(s) * strlen(s));
    char *buffer = memalloc(sizeof(s));

    unsigned _si = 0;
    unsigned _ti = 0;
    unsigned _bi = 0;

    if (s[0]==delim) {
        buffer[0] = delim;
        buffer[1] = '\0';
        char *newbuffer = memalloc(sizeof(s));
        strcpy(newbuffer, buffer);
        tokens[_ti] = newbuffer;

        _bi = 0;
        _si++;
        _ti++;
        free(buffer);
        memset(buffer, 0, sizeof(buffer));
        memset(newbuffer, 0, sizeof(s));
    }

    while (s[_si] != '\0') {
        if (s[_si] == delim) {
            if (strcmp(buffer, "")) {
                _si++;
                _bi=0;
                continue;
            }
            buffer[_bi] = '\0';
            char *newbuffer = memalloc(sizeof(s));
            strcpy(newbuffer, buffer);
            tokens[_ti] = newbuffer;

            _bi = 0;
            _si++;
            _ti++;
            free(buffer);
            memset(buffer, 0, sizeof(buffer));
            memset(newbuffer, 0, sizeof(s));
        } else {
            buffer[_bi] = s[_si];
            _bi++;
            _si++;
        }
    }

    buffer[_bi] = '\0';
    char *newbuffer = memalloc(sizeof(s));
    strcpy(newbuffer, buffer);
    tokens[_ti] = newbuffer;
    free(buffer);
    tokens[_ti + 1] = NULL;

    free(tokens);
    return tokens;
}



char* strcpy(char* dest, const char* src) {
    char* original_dest = dest;
    while ((*dest++ = *src++) != '\0');
    return original_dest;
}

char* strdup(const char* str) {
    size_t length = strlen(str);
    char* new_str = (char*)memalloc(length + 1);  
    if (new_str != NULL) {
        strcpy(new_str, str); 
    }
    return new_str;
}

void to_upper(char *lower, const char *str) {
    unsigned i = 0;
    while (str[i] != '\0') {
        char currentChar = str[i];

        if (currentChar >= 'a' && currentChar <= 'z') {
            lower[i] = currentChar + ('A' - 'a');
        } else {
            lower[i] = currentChar;
        }
        i++;
    }
    lower[i] = '\0';
}

void to_lower(char *lower, const char *str) {
    unsigned i = 0;
    while (str[i] != '\0') {
        char currentChar = str[i];
        
        if (currentChar >= 'A' && currentChar <= 'Z') {
            lower[i] = currentChar - 'A' + 'a';
        } else {
            lower[i] = currentChar;
        }

        i++;
    }
    lower[i] = '\0';
}


int strcmp(const char* s1, const char* s2) {
    if (strlen(s1) != strlen(s2)) {return 0;}
    for (unsigned i=0; i<strlen(s1); i++) {
        if (s1[i] != s2[i]) { return 0; }  
    }
    return 1;
}

char *strncat (char *s1, const char *s2, size_t n) {
    size_t s1size = strlen(s1);
    char *s = memalloc(sizeof(char)*(strlen(s1) + n + 1));

    for (unsigned _i=0;_i<s1size;_i++) {
        s[_i] = s1[_i];
    }
    
    for (unsigned _j=0;_j<n;_j++) {
        s[s1size+_j] = s2[_j];
    }
    s[s1size+n]='\0';
    s1 = s;
    return s;
}

char* unix_to_time(uint32_t unix_time) {
    char* time_str = memalloc(9);

    if (time_str == NULL) {
        return NULL;
    }

    uint32_t hours = unix_time / 3600;
    unix_time %= 3600;

    uint32_t minutes = unix_time / 60;
    unix_time %= 60;

    uint32_t seconds = unix_time;

    // Manually build the time string
    time_str[0] = '0' + (hours / 10);
    time_str[1] = '0' + (hours % 10);
    time_str[2] = ':';
    time_str[3] = '0' + (minutes / 10);
    time_str[4] = '0' + (minutes % 10);
    time_str[5] = ':';
    time_str[6] = '0' + (seconds / 10);
    time_str[7] = '0' + (seconds % 10);
    time_str[8] = '\0';

    return time_str;
}

char* intoa(int i, char b[]){
    char const digit[] = "0123456789";
    char* p = b;
    if(i<0){
        *p++ = '-';
        i *= -1;
    }
    int shifter = i;
    do{ 
        ++p;
        shifter = shifter/10;
    }while(shifter);
    *p = '\0';
    do{ 
        *--p = digit[i%10];
        i = i/10;
    }while(i);
    return b;
}

char* formatBytes(uint64_t bytes) {
    static const char* units[] = {"B", "KB", "MB", "GB"};
    static char result[32];  

    int unitIndex = 0;

    while (bytes >= 1024 && unitIndex < sizeof(units) / sizeof(units[0]) - 1) {
        bytes /= 1024;
        unitIndex++;
    }

    char bytesStr[32];
    intoa(bytes, bytesStr);

    result[0] = '\0';
    strncat(result, bytesStr, 32);
    strncat(result, " ", 1);
    strncat(result, units[unitIndex], 2);

    return result;
}

char *format(const char *format, ...) {
    va_list args;
    va_start(args, format);
    char *outstr=memalloc(512);

    while (*format != '\0') {
        if (*format == '%') {
            format++;

            switch (*format) {
                case 'd': {
                    int value = va_arg(args, int);
                    char buffer[16];
                    itoa(value, buffer, 10);
                    strncat(outstr,buffer,strlen(buffer));
                    break;
                }
                case 's': {
                    const char *str = va_arg(args, const char *);
                    strncat(outstr,str,sizeof(str)/sizeof(char*));
                    break;
                }
                case 'x': {
                    int value = va_arg(args, int);
                    char buffer[16];
                    itoa(value, buffer, 16);
                    strncat(outstr,"0x",2);
                    strncat(outstr,buffer,sizeof(buffer)/sizeof(char));
                    break;
                }
                case 'c': {
                    char value = va_arg(args, int);
                    char buffer[2];
                    buffer[0]=value;
                    buffer[1]='\0';
                    strncat(outstr,buffer,sizeof(buffer)/sizeof(char));
                    break;
                }
                default:
                    strncat(outstr,"%",1);
                    char buffer[2];
                    buffer[0]=*format;
                    buffer[1]='\0';
                    strncat(outstr,buffer,2);
            }
        } else {
            char buffer[2];
            buffer[0]=*format;
            buffer[1]='\0';
            strncat(outstr,buffer,2);
        }
    
        format++;
    }

    va_end(args);
    return outstr;
}