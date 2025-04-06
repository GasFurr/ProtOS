#ifndef IO_H
#define IO_H

#include <stdint.h>

// Read byte from specified port
static inline uint8_t inb(uint16_t port) {
  uint8_t ret;
  __asm__ volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
  return ret;
}

// Write byte to specified port
static inline void outb(uint16_t port, uint8_t val) {
  __asm__ volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

struct interrupt_frame {
  uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
  uint32_t gs, fs, es, ds;
  uint32_t int_num, err_code;
  uint32_t eip, cs, eflags, user_esp, ss;
};

#endif
