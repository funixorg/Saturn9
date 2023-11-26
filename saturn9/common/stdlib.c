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

char** str_split(char* a_str, const char a_delim) {
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
  char *s = s1;
  /* Find the end of S1.  */
  s1 += strlen (s1);
  size_t ss = strlen (s2);
  s1[ss] = '\0';
  memcpy (s1, s2, ss);
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
    do{ //Move to where representation ends
        ++p;
        shifter = shifter/10;
    }while(shifter);
    *p = '\0';
    do{ //Move back, inserting digits as u go
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