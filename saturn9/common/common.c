#include <common.h>

void hcf() {
    asm ("cli");
    for (;;) {
        asm ("hlt");
    }
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