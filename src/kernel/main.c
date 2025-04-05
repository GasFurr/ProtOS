#include "graphic.h"
#include "kernel.h"
#include "serial.h"
#include "text.h"
#include <gdt.h>
#include <stddef.h>
#include <stdint.h>

void KMain() {
  // init.c should call function KMain:
  serial_puts("\nProtOS Main Kernel started.\n");
  // Clearing screen and setting up cursor.
  // Well, its all just tests, "do everything initialized?"
  clear_screen(BLACK);
  set_cursor_pos(1, 0);
  // And then text output;
  KOutput("\tMain kernel started.\n");
  set_text_color(WHITE, BLACK);

  KOutput(
      " No difference between day and night\n No thoughts at all\n No dreams\n "
      "Day and night are the same - blank \n Precisely like death.");
  // Halt and catch fire.
  hcf();
}
