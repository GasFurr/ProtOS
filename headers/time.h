// time.h - Timer and delay functions
#ifndef TIME_H
#define TIME_H

#define PIT_FREQUENCY 1193180
#define PIT_CHANNEL0 0x40
#define PIT_CMD 0x43

#include <stdint.h>

// Initialize PIT timer at 1000Hz (1ms interrupts)
void timer_init(uint32_t frequency);

// Get current system ticks since boot
uint32_t timer_ticks();

// Blocking sleep functions
void sleep(uint32_t milliseconds);
void sleep_ticks(uint32_t ticks);

void timer_handler();

#endif
