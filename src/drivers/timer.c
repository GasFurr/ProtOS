/* drivers/timer.c */
#include "../kernel.h"

volatile uint32_t timer_ticks = 0;
static uint32_t last_remaining = 0;

void timer_handler(void) {
    timer_ticks++;
    
    if(shutdown_timeout > 0) {
        uint32_t remaining = (shutdown_timeout - timer_ticks) / 100;
        if(remaining <= 10 && remaining != last_remaining) {
            print("\b\b"); // Backspace previous number
            if(remaining < 10) print(" ");
            print_dec(remaining);
            last_remaining = remaining;
        }
    }
    
    outb(0x20, 0x20);
}

void timer_init(uint16_t freq) {
    uint16_t divisor = 1193180 / freq;
    outb(0x43, 0x36);
    outb(0x40, divisor & 0xFF);
    outb(0x40, divisor >> 8);
}
