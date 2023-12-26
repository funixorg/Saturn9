#ifndef SHELL_H
#define SHELL_H

#define MAX_ENTRIES 1024

struct CommandEntry {
    const char* command;
    void (*function)();
};

void SHELL_add_command(const char* command, void (*function)());
void SHELL_execute_command(const char* command, char **cmdnargs);
void SHELL_shellrun();
void SHELL_shell_init();

#endif
