// time.c - Programmable Interval Timer (PIT) driver
#include "time.h"
#include "idt.h"
#include "io.h"
#include "serial.h"
#include "text.h"

static volatile uint32_t system_ticks = 0;

// Proper interrupt handler with attributes
void timer_handler() {
  system_ticks++;
  outb(0x20, 0x20); // Send EOI to PIC
}

void timer_init(uint32_t freq) {
  uint16_t divisor = PIT_FREQUENCY / freq;

  // Configure PIT channel 0
  outb(PIT_CMD, 0x36); // Channel 0, lo/hi byte, square wave
  outb(PIT_CHANNEL0, divisor & 0xFF);
  outb(PIT_CHANNEL0, (divisor >> 8) & 0xFF);

  // Register correct handler
  idt_set_entry(32, (uint32_t)timer_handler, 0x08, 0x8E);
}

uint32_t timer_ticks() { return system_ticks; }

void sleep_ticks(uint32_t ticks) {
  uint32_t end = system_ticks + ticks;
  while (system_ticks < end)
    ;
}

void sleep(uint32_t ms) {
  sleep_ticks(ms);
  serial_puts("Slept\n");
}
