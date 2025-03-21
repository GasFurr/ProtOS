/* commands.c - Command processor implementation */
#include <stddef.h>
#include "commands.h"
#include "kernel.h"

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
    {"clear", cmd_clear},
    {"test", cmd_test},
    {"help", cmd_help},
    {"shutdown", cmd_shutdown},
    {NULL, NULL}
};

static uint8_t shutdown_pending = 0;
uint32_t shutdown_timeout = 0;


/* Command implementations */
static void cmd_clear(void) {
    clear_screen();
    print("ProtOS >");
}

static void cmd_test(void) {
    print("Hello User!\n");
    print("ProtOS^>");
}

static void cmd_help(void) {
    print("Available commands: clear, test, help, shutdown\n");
    print("ProtOS^>");
}

static void cmd_shutdown(void) {
    print("Confirm shutdown? (Y/N)\n");
    print("'y' will shutdown in 10 seconds\n");
    shutdown_pending = 1;
    buffer_index = 0;
    mem_set(input_buffer, 0, sizeof(input_buffer));
}

void execute_command(void) {
    print("\n");

    /* Handle empty command */
    if(buffer_index == 0) {
        print("ProtOS*> ");
        return;
    }

    /* Check for shutdown confirmation */
    if(shutdown_pending) {
        char response = input_buffer[0];
        
        if(response == 'Y' || response == 'y') {
            if(response == 'y') {
                print("Shutting down in 10 seconds...\n");
                shutdown_timeout = timer_ticks + 1000;
                // this shit is absolutely not working.
            } else {
                print("Shutting down immediately...\n");
                outw(0x604, 0x2000);
            }
        } 
        else if(response == 'N' || response == 'n') {
            print("Shutdown cancelled.\n");
            print("ProtOS->");
        } 
        else {
            print("Invalid response. Use Y/y/N/n\n");
            print("ProtOS~>");
        }
        /* Reset shutdown state */
        shutdown_pending = 0;
        buffer_index = 0;
        mem_set(input_buffer, 0, sizeof(input_buffer));
        print("ProtOS/> ");
        return;
    }

    /* Handle scheduled shutdown */
    if(shutdown_timeout > 0 && timer_ticks >= shutdown_timeout) {
        outw(0x604, 0x2000);
        return;
    }

    /* Normal command processing */
    for(const struct command *cmd = commands; cmd->name; cmd++) {
        if(strcmp(input_buffer, cmd->name) == 0) {
            cmd->handler();
            buffer_index = 0;
            mem_set(input_buffer, 0, sizeof(input_buffer));
            return;
        }
    }

    /* Unknown command handling */
    print("Unknown command: ");
    print(input_buffer);
    print("\n");
    buffer_index = 0;
    mem_set(input_buffer, 0, sizeof(input_buffer));
    print("ProtOS!> ");
}
