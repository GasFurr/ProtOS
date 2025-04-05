// serial.h - Serial port driver
#ifndef SERIAL_H
#define SERIAL_H

void serial_init();
void serial_putc(char c);
void serial_puts(const char *str);

#endif
