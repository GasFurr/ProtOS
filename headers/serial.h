// serial.h - Serial port driver
#ifndef SERIAL_H
#define SERIAL_H
#include <stdint.h>

void serial_init();
void serial_putc(char c);
void serial_puts(const char *str);
void serial_puthex(uint32_t val);

#endif
