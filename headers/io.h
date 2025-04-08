#ifndef IO_H
#define IO_H

#include <stdint.h>

#define PS2_STATUS 0x64
#define PS2_DATA 0x60

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

static inline void outw(uint16_t port, uint16_t val) {
  __asm__ volatile("outw %w0, %w1" : : "a"(val), "Nd"(port));
}

static inline uint8_t ps2_read_status(void) { return inb(PS2_STATUS); }

struct interrupt_frame {
  uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
  uint32_t gs, fs, es, ds;
  uint32_t int_num, err_code;
  uint32_t eip, cs, eflags, user_esp, ss;
};

#endif
