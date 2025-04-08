#define PIC1_CMD 0x20
#define PIC1_DATA 0x21
#define PIC2_CMD 0xA0
#define PIC2_DATA 0xA1
#define ICW1_INIT 0x11
#define ICW4_8086 0x01
#include "io.h"

void remap_pic();
static inline void io_wait() {
  outb(0x80, 0); // Write to unused port for delay
}
