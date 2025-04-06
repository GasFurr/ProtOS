#include "io.h"
#include <stdint.h>

#define PIC1_CMD 0x20
#define PIC1_DATA 0x21
#define PIC2_CMD 0xA0
#define PIC2_DATA 0xA1
#define ICW1_INIT 0x11 // Initialization command
#define ICW4_8086 0x01 // 8086/88 mode

void pic_remap() {
  /* Save original masks */
  uint8_t mask1 = inb(PIC1_DATA);
  uint8_t mask2 = inb(PIC2_DATA);

  /* Start initialization sequence */
  outb(PIC1_CMD, ICW1_INIT | ICW4_8086); // Begin PIC1 init
  outb(PIC2_CMD, ICW1_INIT | ICW4_8086); // Begin PIC2 init

  /* Set vector offsets */
  outb(PIC1_DATA, 0x20); // PIC1 interrupts start at 0x20 (32)
  outb(PIC2_DATA, 0x28); // PIC2 interrupts start at 0x28 (40)

  /* Configure cascade */
  outb(PIC1_DATA, 0x04); // PIC1 has slave at IRQ2 (00000100)
  outb(PIC2_DATA, 0x02); // PIC2 is cascaded via IRQ1

  /* Set 8086 mode */
  outb(PIC1_DATA, ICW4_8086);
  outb(PIC2_DATA, ICW4_8086);

  /* Restore masks with timer enabled */
  outb(PIC1_DATA, 0xFE); // 11111110 - Only IRQ0 (timer) enabled
  outb(PIC2_DATA, 0xFF); // 11111111 - All PIC2 interrupts disabled

  /* Wait for commands to complete */
  __asm__ volatile("jmp 1f\n1: jmp 1f\n1:"); // Short delay
}
