// idt.c
#include "idt.h"
#include "io.h"
#include "serial.h"
#include "time.h"

struct idt_entry idt[256];
struct idt_ptr idtp;

extern void idt_load();

void idt_set_entry(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
  idt[num].base_low = base & 0xFFFF;
  idt[num].base_high = (base >> 16) & 0xFFFF;
  idt[num].sel = sel;
  idt[num].always0 = 0;
  idt[num].flags = flags | 0x60; // Present, ring 0
}
extern struct idt_ptr idt_descriptor; // Reference ASM symbol

void idt_init() {
  idt_descriptor.limit = sizeof(struct idt_entry) * 256 - 1; // Add 'struct'
  idt_descriptor.base = (uint32_t)&idt;

  // Set all IDT entries
  for (int i = 0; i < 256; i++) {
    idt_set_entry(i, 0, 0x08, 0x8E);
    // 32 = IRQ0 vector, 0x8E = present+interrupt gate
  }

  idt_set_entry(32, (uint32_t)timer_handler, 0x08, 0x8E);

  // Set CPU exception handlers (0-31)
  extern void isr0(), isr1(), isr2(), isr3(), isr4(), isr5(), isr6(), isr7(),
      isr8(), isr9(), isr10(), isr11(), isr12(), isr13(), isr14(), isr15(),
      isr16(), isr17(), isr18(), isr19(), isr20(), isr21(), isr22(), isr23(),
      isr24(), isr25(), isr26(), isr27(), isr28(), isr29(), isr30(),
      isr31(); // Add all
#define SET_IDT(n) idt_set_entry(n, (uint32_t)isr##n, 0x08, 0x8E)

  SET_IDT(0);
  SET_IDT(1);
  SET_IDT(2);
  SET_IDT(3);
  SET_IDT(4);
  SET_IDT(5);
  SET_IDT(6);
  SET_IDT(7);
  SET_IDT(8);
  SET_IDT(9);
  SET_IDT(10);
  SET_IDT(11);
  SET_IDT(12);
  SET_IDT(13);
  SET_IDT(14);
  SET_IDT(15);
  SET_IDT(16);
  SET_IDT(17);
  SET_IDT(18);
  SET_IDT(19);
  SET_IDT(20);
  SET_IDT(21);
  SET_IDT(22);
  SET_IDT(23);
  SET_IDT(24);
  SET_IDT(25);
  SET_IDT(26);
  SET_IDT(27);
  SET_IDT(28);
  SET_IDT(29);
  SET_IDT(30);
  SET_IDT(31);

  idt_load();
}

void isr_handler(struct registers *regs) {
  const char *messages[32] = {"Divide Error",
                              "Debug",
                              "NMI",
                              "Breakpoint",
                              "Overflow",
                              "Bound Range",
                              "Invalid Opcode",
                              "Device Not Available",
                              "Double Fault",
                              "Coprocessor Segment",
                              "Invalid TSS",
                              "Segment Not Present",
                              "Stack Fault",
                              "General Protection Fault",
                              "Page Fault",
                              "Reserved",
                              "x87 FPU",
                              "Alignment Check",
                              "Machine Check",
                              "SIMD FP",
                              "Virtualization",
                              "Control Protection",
                              "Reserved",
                              "Reserved",
                              "Reserved",
                              "Reserved",
                              "Reserved",
                              "Reserved",
                              "Hypervisor Injection",
                              "VMM Communication",
                              "Security Exception",
                              "Reserved"};

  serial_puts("\n! EXCEPTION: ");
  serial_puts(messages[regs->int_num]);
  serial_puts("\nINT: 0x");
  serial_puthex(regs->int_num);
  serial_puts(" ERR: 0x");
  serial_puthex(regs->err_code);
  serial_puts(" EIP: 0x");
  serial_puthex(regs->eip);
  serial_puts(" CR2: 0x");
  serial_puthex(read_cr2());
  serial_puts("\n");

  // Dump registers
  serial_puts("EAX:0x");
  serial_puthex(regs->eax);
  serial_puts(" EBX:0x");
  serial_puthex(regs->ebx);
  serial_puts(" ECX:0x");
  serial_puthex(regs->ecx);
  serial_puts(" EDX:0x");
  serial_puthex(regs->edx);
  serial_puts("\nESI:0x");
  serial_puthex(regs->esi);
  serial_puts(" EDI:0x");
  serial_puthex(regs->edi);
  serial_puts(" EBP:0x");
  serial_puthex(regs->ebp);
  serial_puts(" ESP:0x");
  serial_puthex(regs->esp);
  serial_puts("\n");

  while (1)
    ; // Halt
}
