#ifndef TERMINAL_H
#define TERMINAL_H

#define PROMPT "#{0x3c3c3c}/> "


#define MAX_ENTRIES 128

struct CommandEntry {
    const char* command;
    void (*function)();
};

void add_command(const char* command, void (*function)());

void execute_command(const char* command);

void shell();

void terminal_init();

#endif
