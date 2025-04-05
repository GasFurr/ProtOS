// serial.c - Serial port implementation
#include "serial.h"
#include <stddef.h>

#define COM1_PORT 0x3F8

static void outb(unsigned short port, unsigned char val) {
  asm volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

static unsigned char inb(unsigned short port) {
  unsigned char ret;
  asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
  return ret;
}

void serial_init() {
  outb(COM1_PORT + 1, 0x00); // Disable interrupts
  outb(COM1_PORT + 3, 0x80); // Enable DLAB (set baud rate divisor)
  outb(COM1_PORT + 0, 0x0C); // Set divisor to 12 (9600 baud)
  outb(COM1_PORT + 1, 0x00);
  outb(COM1_PORT + 3, 0x03); // 8 bits, no parity, one stop bit
  outb(COM1_PORT + 2, 0xC7); // Enable FIFO, clear them, 14-byte threshold
}

void serial_putc(char c) {
  while ((inb(COM1_PORT + 5) & 0x20) == 0)
    ; // Wait for transmit buffer empty
  outb(COM1_PORT, c);

  // Add CR when we get LF
  if (c == '\n') {
    serial_putc('\r');
  }
}

void serial_puts(const char *str) {
  for (size_t i = 0; str[i]; i++) {
    serial_putc(str[i]);
  }
}
