#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

extern char input_buffer[256];
extern uint16_t buffer_index;

void keyboard_init();
void keyboard_handler(uint8_t scancode);
void keyboard_interrupt_handler(void);

#endif
