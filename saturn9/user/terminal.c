#include <terminal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <idt.h>

//for (i = 0; *(tokens + i); i++)

struct CommandEntry command_array[MAX_ENTRIES];

void add_command(const char* command, void (*function)()) {
    size_t i;
    for (i = 0; i < MAX_ENTRIES; ++i) {
        if (command_array[i].command == NULL) {
            command_array[i].command = command;
            command_array[i].function = function;
            break;
        }
    }
}

void execute_command(const char* command) {
    for (size_t i = 0; i < MAX_ENTRIES; ++i) {
        if (command_array[i].command != NULL && strcmp(command_array[i].command, command)) {
            command_array[i].function();
            return;
        }
    }
    printf("#{0xff0000}Unknown command: #{0xff4e4e}%s\n", command);
}


void execvp(char** tokens) {
  char *cmd = tokens[0];
  
  execute_command(cmd);
}


void shell() {
  char *line=readline(PROMPT);
  if (strlen(line)>0) {
    char** tokens =tok_split(line,' ');
    execvp(tokens);
  }
}

void terminal_init() {
  add_command("clear", clear_screen);
  for (;;) { shell(); }
}
