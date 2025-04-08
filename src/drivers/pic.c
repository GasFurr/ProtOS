#include "pic.h"
#include "io.h"
#include <stdint.h>

#define PIC1_CMD 0x20
#define PIC1_DATA 0x21
#define PIC2_CMD 0xA0
#define PIC2_DATA 0xA1
#define ICW1_INIT 0x11
#define ICW4_8086 0x01

// Helper for I/O delays

void pic_remap(void) {
  outb(PIC1_CMD, 0x11);
  outb(PIC2_CMD, 0x11); /* ICW1: Initialize */
  io_wait();
  outb(PIC1_DATA, 0x20);
  outb(PIC2_DATA, 0x28); /* ICW2: Vector offsets */
  outb(PIC1_DATA, 0x04);
  io_wait();
  outb(PIC2_DATA, 0x02); /* ICW3: Master/Slave */
  outb(PIC1_DATA, 0x01);
  io_wait();
  outb(0x64, 0xAE); // Enable PS/2 port 1
  while (inb(0x64) & 0x02)

    outb(PIC2_DATA, 0x01); /* ICW4: 8086 mode */
  outb(PIC1_DATA, 0xFD);   /* Unmask IRQ0 & IRQ1 */
  io_wait();
  ; // Wait until ready
}
