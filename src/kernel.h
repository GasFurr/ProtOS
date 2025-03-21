/* kernel.h - Core OS header */
#pragma once

#include <stdint.h>

/* ==================== VGA Interface ==================== */
#define VGA_WIDTH  80
#define VGA_HEIGHT 25

void print(const char *str);
void print_char(char c);
void clear_screen(void);

/* ==================== Interrupt Handling ==================== */
typedef void (*isr_handler_t)(uint32_t);
extern void default_handler(void);
extern void timer_interrupt_wrapper(void);
extern void keyboard_interrupt_wrapper(void);
void idt_init(void);
void pic_remap(void);
void enable_interrupts(void);
extern void timer_handler(void);
extern void keyboard_interrupt_handler(void);

/* ==================== More handlers ==================== */

void print_dec(uint32_t num);
extern uint32_t shutdown_timeout;
static inline void outw(uint16_t port, uint16_t value) {
    __asm__ __volatile__("outw %0, %1" : : "a"(value), "Nd"(port));
}
extern volatile uint32_t timer_ticks;
void default_handler(void);
void isr_handler(uint32_t int_no);
void irq_handler(uint32_t int_no);

/* ==================== Hardware I/O ==================== */
#define PIC1_CMD  0x20
#define PIC1_DATA 0x21
#define PIC2_CMD  0xA0
#define PIC2_DATA 0xA1

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ __volatile__("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}
extern void load_gdt(void);
extern void timer_init(uint16_t freq);
static inline void outb(uint16_t port, uint8_t value) {
    __asm__ __volatile__("outb %0, %1" : : "a"(value), "Nd"(port));
}

extern char input_buffer[256];
extern uint16_t buffer_index;

/* ==================== Memory & String ==================== */
void mem_set(void *ptr, uint8_t value, uint32_t size);
int strcmp(const char *s1, const char *s2);

/* ==================== Driver Interfaces ==================== */
void keyboard_init(void);
void keyboard_handler(uint8_t scancode);
void timer_init(uint16_t freq);
