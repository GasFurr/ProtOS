
/* commands.c - Command processor implementation */
#include "testshell.h"
#include <io.h>
#include <stddef.h>
#include <string.h>
#include <text.h>

/* External declarations */
extern volatile uint32_t timer_ticks;
extern char input_buffer[256];
extern uint16_t buffer_index;

/* Command structure */
struct command {
  const char *name;
  void (*handler)(void);
};

/* Command handlers forward declarations */
static void cmd_clear(void);
static void cmd_test(void);
static void cmd_help(void);
static void cmd_shutdown(void);

/* Command table */
static const struct command commands[] = {
    {"clear", cmd_clear}, {"test", cmd_test}, {"help", cmd_help}, {NULL, NULL}};

/* Command implementations */
static void cmd_clear(void) {
  clear_screen();
  KOutput("ProtOS >");
}

static void cmd_test(void) {
  KOutput("Hello User!\n");
  KOutput("ProtOS^>");
}

static void cmd_help(void) {
  KOutput("Available commands: clear, test, help, shutdown\n");
  KOutput("ProtOS^>");
}

void execute_command(void) {
  KOutput("\n");

  /* Handle empty command */
  if (buffer_index == 0) {
    KOutput("ProtOS*> ");
    return;
  }

  /* Normal command processing */
  for (const struct command *cmd = commands; cmd->name; cmd++) {
    if (strcmp(input_buffer, cmd->name) == 0) {
      cmd->handler();
      buffer_index = 0;
      memset(input_buffer, 0, sizeof(input_buffer));
      return;
    }
  }

  /* Unknown command handling */
  KOutput("Unknown command: ");
  KOutput(input_buffer);
  KOutput("\n");
  buffer_index = 0;
  memset(input_buffer, 0, sizeof(input_buffer));
  KOutput("ProtOS!> ");
}
