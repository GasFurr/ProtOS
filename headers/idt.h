// idt.h
#ifndef IDT_H
#define IDT_H

#include <stdint.h>

struct registers {
  uint32_t ds;
  uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
  uint32_t int_num, err_code;
  uint32_t eip, cs, eflags, user_esp, ss;
};

struct idt_entry {
  uint16_t base_low;
  uint16_t sel;
  uint8_t always0;
  uint8_t flags;
  uint16_t base_high;
} __attribute__((packed));

struct idt_ptr {
  uint16_t limit;
  uint32_t base;
} __attribute__((packed));

static inline uint32_t read_cr2(void) {
  uint32_t cr2;
  __asm__ __volatile__("mov %%cr2, %0" : "=r"(cr2));
  return cr2;
}

void idt_init();
void idt_set_entry(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);
void isr_handler(struct registers *regs);
void pic_remap();
#define IRQ0_VECTOR 0x20

#endif // IDT_H
