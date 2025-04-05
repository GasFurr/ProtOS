// time.c - Programmable Interval Timer (PIT) driver
#include "time.h" // Time driver header
#include "idt.h"
#include "io.h" // port IO interface

#define PIT_FREQUENCY 1193180
#define PIT_CHANNEL0 0x40
#define PIT_CMD 0x43

static volatile uint64_t system_ticks = 0;

static void timer_callback() { system_ticks++; }

void timer_init(uint32_t freq) {
  uint16_t divisor = PIT_FREQUENCY / freq;

  // Configure PIT channel 0
  outb(PIT_CMD, 0x36); // Channel 0, lo/hi byte, square wave
  outb(PIT_CHANNEL0, divisor & 0xFF);
  outb(PIT_CHANNEL0, (divisor >> 8) & 0xFF);

  // Register IRQ0 handler
  idt_set_entry(32, (uint32_t)timer_callback, 0x08, 0x8E);
}

uint64_t timer_ticks() { return system_ticks; }

void sleep_ticks(uint32_t ticks) {
  uint64_t end = system_ticks + ticks;
  while (system_ticks < end)
    ;
}

void sleep(uint32_t ms) {
  sleep_ticks(ms); // 1ms per tick;
}
