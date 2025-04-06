#include "graphic.h"
#include "kb.h"
#include "kernel.h"
#include "serial.h"
#include "text.h"
#include "time.h"
#include <gdt.h>
#include <stddef.h>
#include <stdint.h>

void KMain() {
  // init.c should call function KMain:
  serial_puts("\nProtOS Main Kernel started.\n");
  // Clearing screen and setting up cursor.
  // Well, its all just tests, "do everything initialized?"
  clear_screen();
  set_cursor_pos(1, 0);
  // And then text output;
  sleep(500);
  KOutput("\tMain kernel started.\n");
  set_text_color(WHITE, BLACK);
  sleep(1000);
  KOutput("No difference between day and night\n");
  sleep(1100);
  KOutput("No thoughts at all\n");
  sleep(900);
  KOutput("No dreams\n");
  sleep(1000);
  KOutput("Day and night are the same -");
  sleep(700);
  KOutput(" blank.\n");
  sleep(1100);
  set_text_color(RED, BLACK);
  KOutput_center("Precisely like death.");
  sleep(1000);
  clear_screen();

  // Halt and catch fire.
  while (1) {
    char c = keyboard_read();
    if (c != 0) {
      char str[2] = {c, 0};
      KOutput(str);
    }
  }
}
