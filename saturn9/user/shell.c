#include <shell.h>
#include <stdio.h>
#include <terminal.h>
#include <stdlib.h>
#include <stdint.h>
#include <filesystem.h>
#include <idt.h>
#include <mem.h>
#include <titan.h>

struct CommandEntry command_array[MAX_ENTRIES];

Directory *current_dir;

void SHELL_add_command(const char* command, void (*function)()) {
    size_t i;
    for (i = 0; i < MAX_ENTRIES; ++i) {
        if (command_array[i].command == NULL) {
            command_array[i].command = command;
            command_array[i].function = function;
            break;
        }
    }
}

void SHELL_execute_command(const char* command, char **cmdnargs) {
    for (size_t i = 0; i < MAX_ENTRIES; ++i) {
        if (command_array[i].command != NULL && strcmp(command_array[i].command, command)) {
            command_array[i].function(cmdnargs);
            return;
        }
    }
    printf("#{0xff0000}Unknown command: #{0xff4e4e}%s\n", command);
}


void SHELL_execvp(char** tokens) {
  char **cmdnargs = tokens;
  
  SHELL_execute_command(cmdnargs[0], cmdnargs);
}



void SHELL_shellrun() {
  char *prompt = VFS_read_path("/sys/shell/prompt");
  if (!prompt) {
  }
  printf(prompt, "unity", "/");
  char *line=readline();
  if (strlen(line)>0) {
    char** tokens =tok_split(line,' ');
    SHELL_execvp(tokens);
  }
}

/*void SHELL_temp_cat(char **cmdnargs) {
  if (!cmdnargs[1]) {
    return;
  }
  char *content = VFS_read_path(cmdnargs[1]);
  if (content) { printf("%s\n", content); }
  else { printf("#{0xff0000}`#{0xb14e4e}%s#{0xff0000}` file not found!\n", cmdnargs[1]); }
  free(content);
}*/

void SHELL_temp_clear(char **cmdnargs) {
  clear_screen();
}

void SHELL_temp_lsdir(char **cmdnargs) {
  if (!cmdnargs[1]) {
    FileList *flist = VFS_list_dir("/");
    if (!flist) {
      printf("#{0xff0000}`#{0xb14e4e}%s#{0xff0000}` dir not found!\n", "/");
      return;
    }
    for (unsigned _i=0; _i<flist->file_count; _i++) {
      printf("%s   ", flist->paths[_i]);
    }
    for (unsigned _i=0; _i<flist->dir_count; _i++) {
      printf("%s/   ", flist->dirs[_i]);
    }
    printf("\n");
    return;
  }

  FileList *flist = VFS_list_dir(cmdnargs[1]);
  if (!flist) {
    printf("#{0xff0000}`#{0xb14e4e}%s#{0xff0000}` dir not found!\n", cmdnargs[1]);
    return;
  }
  for (unsigned _i=0; _i<flist->file_count; _i++) {
    printf("%s   ", flist->paths[_i]);
  }
  for (unsigned _i=0; _i<flist->dir_count; _i++) {
    printf("%s/   ", flist->dirs[_i]);
  }
  printf("\n");
}

void SHELL_run_exec(char **cargs) {
  char *base_path = format("/sys/bin/%s", cargs[0]);
  char **args = memalloc(sizeof(char)*32);
  if (cargs[1]) {
    args[0] = cargs[1];
  } else {
    args = NULL;
  }
  TITAN_run_exe(base_path, args);
}

void SHELL_init_bin() {
  Directory *root = VFS_find_dir("/");
  for (unsigned _i=0; _i<root->dir_count;_i++) {
    if (strcmp(root->directories[_i].path, "sys")) {
      for (unsigned _j=0; _j<root->directories[_i].dir_count;_j++) {
        if (strcmp(root->directories[_i].directories[_j].path, "bin")) {
          for (unsigned _k=0;_k<root->directories[_i].directories[_j].file_count; _k++) {
            File bin = root->directories[_i].directories[_j].files[_k];
            SHELL_add_command(bin.path, SHELL_run_exec);
          }
        }
      }
    }
  }
}

void SHELL_shell_init() {
  current_dir = VFS_find_dir("/");
  SHELL_init_bin();
  //SHELL_add_command("pan", SHELL_temp_cat);
  SHELL_add_command("ls", SHELL_temp_lsdir);
  for (;;) {
    SHELL_shellrun();
    TERM_set_foreground(TERM_get_default_fg());
    TERM_set_background(TERM_get_default_bg());
  }
}
