#include "serial.h"
// Kernel headers
void KMain();
// halt and catch fire instruction.
static void hcf(void) {
  serial_puts("Kernel Halted");
  for (;;) {
    asm("hlt");
  }
}
