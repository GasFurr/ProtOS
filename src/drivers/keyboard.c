/* Keyboard Driver */
#include "../kernel.h"

__attribute__((naked)) void keyboard_interrupt_wrapper(void) {
    __asm__("pusha");
    __asm__("call keyboard_interrupt_handler");
    __asm__("popa");
    __asm__("iretl");
}

void keyboard_interrupt_handler(void) {
    uint8_t scancode = inb(0x60);
    keyboard_handler(scancode);
    outb(0x20, 0x20);
}

void keyboard_init(void) {
    outb(PIC1_DATA, inb(PIC1_DATA) & ~0x02);
}
