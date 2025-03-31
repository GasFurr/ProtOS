#include <kernel/multiboot_tags.h>
#define PAGE_SIZE 4096

static uint8_t memory_bitmap[1024 * 1024] = {0}; // 4MB bitmap for 4GB RAM

void bitmap_set(uint64_t page, int used) {
  uint64_t idx = page / 8;
  uint8_t bit = page % 8;
  if (used)
    memory_bitmap[idx] |= (1 << bit);
  else
    memory_bitmap[idx] &= ~(1 << bit);
}

void handle_memory_map(struct multiboot_mmap_entry *entry) {
  if (entry->type != 1)
    return; // Only track available RAM

  uint64_t start = entry->addr;
  uint64_t end = entry->addr + entry->len;

  // Mark entire region as free
  for (uint64_t addr = start; addr < end; addr += PAGE_SIZE) {
    uint64_t page = addr / PAGE_SIZE;
    bitmap_set(page, 0);
  }
}
