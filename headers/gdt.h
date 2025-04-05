#pragma once
#include <stdint.h>

// Granularity flags
#define GDT_GRAN_4K 0x80
#define GDT_GRAN_1B 0x00

// Segment types
#define GDT_CODE_EXEC 0x0A
#define GDT_DATA_READ 0x02
#define GDT_DATA_WRITE 0x02

struct gdt_entry {
  uint16_t limit_low;
  uint16_t base_low;
  uint8_t base_middle;
  uint8_t access;      // Contains Type (4 bits), S, DPL, P flags
  uint8_t granularity; // Contains Limit high, Flags, G, D/B, L
  uint8_t base_high;
} __attribute__((packed));

struct gdt_ptr {
  uint16_t limit;
  uint32_t base;
} __attribute__((packed));

extern struct gdt_entry gdt[3];
extern struct gdt_ptr gdtp;

void gdt_flush(uint32_t ptr);
void gdt_init();
void gdt_set_entry(int num, uint32_t base, uint32_t limit, uint8_t access,
                   uint8_t gran);
void gdt_debug();
