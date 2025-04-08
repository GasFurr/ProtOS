#include "bscshl.h"
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
  serial_puts("KMain!\n");
  keyboard_init();
  serial_puts("Keyboard should be initialized\n");
  keyboard_interrupt_handler();
  while (1) {
  }
}
