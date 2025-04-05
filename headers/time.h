// time.h - Timer and delay functions
#ifndef TIME_H
#define TIME_H

#include <stdint.h>

// Initialize PIT timer at 1000Hz (1ms interrupts)
void timer_init(uint32_t frequency);

// Get current system ticks since boot
uint64_t timer_ticks();

// Blocking sleep functions
void sleep(uint32_t milliseconds);
void sleep_ticks(uint32_t ticks);

#endif
