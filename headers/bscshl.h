#ifndef BSCSHL_H
#define BSCSHL_H

#include <stdint.h>

void shell_init();
void shell_process_input(char c);
void shell_execute_command(const char *cmd);
void shell_prompt();
void shell_update();

#endif
