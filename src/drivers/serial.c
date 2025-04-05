// serial.c - Serial port implementation
#include "serial.h"
#include "io.h"
#include <stddef.h>
#include <stdint.h>

#define COM1_PORT 0x3F8

void serial_init() {
  outb(COM1_PORT + 1, 0x00); // Disable interrupts
  outb(COM1_PORT + 3, 0x80); // Enable DLAB (set baud rate divisor)
  outb(COM1_PORT + 0, 0x0C); // Set divisor to 12 (9600 baud)
  outb(COM1_PORT + 1, 0x00);
  outb(COM1_PORT + 3, 0x03); // 8 bits, no parity, one stop bit
  outb(COM1_PORT + 2, 0xC7); // Enable FIFO, clear them, 14-byte threshold
  serial_puts("Serial Output initialized.\n");
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

void serial_puthex(uint32_t val) {
  const char *hex_chars = "0123456789ABCDEF";

  // Prefix
  serial_puts("0x");

  // Process each nibble from MSB to LSB
  for (int i = 28; i >= 0; i -= 4) {
    uint8_t nibble = (val >> i) & 0xF;
    serial_putc(hex_chars[nibble]);
  }
}

void serial_puts(const char *str) {
  for (size_t i = 0; str[i]; i++) {
    serial_putc(str[i]);
  }
}
