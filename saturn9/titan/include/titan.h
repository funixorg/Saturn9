
#include <filesystem.h>

#ifndef TITAN_H
#define TITAN_H

unsigned TITAN_exe_interpreter(File *source, char **args);
void TITAN_run_exe(char *path, char **args);

#endif