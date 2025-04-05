#include "gdt.h"
#include "serial.h"

struct gdt_entry gdt[3] = {0};
struct gdt_ptr gdtp = {0};

void gdt_init() {
  gdtp.limit = sizeof(gdt) - 1;
  gdtp.base = (uint32_t)&gdt;

  // Null descriptor
  gdt_set_entry(0, 0, 0, 0, 0);

  // Code segment - 0x9A (10011010)
  gdt_set_entry(1, 0, 0xFFFFF, // 4GB limit with 4K granularity
                0x9A,          // Present, Ring 0, Code, Exec/Read
                0xCF);         // 4K granular, 32-bit mode

  // Data segment - 0x92 (10010010)
  gdt_set_entry(2, 0, 0xFFFFF,
                0x92, // Present, Ring 0, Data, Read/Write
                0xCF);

  gdt_flush((uint32_t)&gdtp);
}

// Atomic descriptor creation
void gdt_set_entry(int num, uint32_t base, uint32_t limit, uint8_t access,
                   uint8_t gran) {
  gdt[num].base_low = (base & 0xFFFF);
  gdt[num].base_middle = (base >> 16) & 0xFF;
  gdt[num].base_high = (base >> 24) & 0xFF;

  gdt[num].limit_low = (limit & 0xFFFF);
  gdt[num].granularity = ((limit >> 16) & 0x0F);
  gdt[num].granularity = (gran & 0xF0) | ((limit >> 16) & 0x0F);

  gdt[num].access = access;
}

void gdt_debug() {
  serial_puts("\n=== GDT Validation ===\n");

  // Check null descriptor
  if (gdt[0].limit_low != 0 || gdt[0].base_low != 0) {
    serial_puts("NULL DESCRIPTOR CORRUPT!\n");
    return;
  }

  // Code segment validation
  uint8_t code_access = gdt[1].access;
  serial_puts("Code Segment: ");
  serial_puthex(code_access);
  serial_puts((code_access == 0x9A) ? " (VALID)\n" : " (INVALID!)\n");

  // Data segment validation
  uint8_t data_access = gdt[2].access;
  serial_puts("Data Segment: ");
  serial_puthex(data_access);
  serial_puts((data_access == 0x92) ? " (VALID)\n" : " (INVALID!)\n");

  // Check GDTR loading
  asm volatile("sgdt %0" : "=m"(gdtp));
  serial_puts("GDTR Loaded: ");
  serial_puthex(gdtp.base);
  serial_puts(" Limit: ");
  serial_puthex(gdtp.limit);
  serial_puts("\n");

  // In gdt_debug()
  serial_puts("Raw Code Entry: ");
  for (int i = 0; i < 8; i++) {
    serial_puthex(*((uint8_t *)&gdt[1] + i));
    serial_putc(' ');
  }
  serial_puts("\n");
}
