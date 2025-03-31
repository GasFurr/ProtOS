#include <kernel/multiboot_tags.h>
#include <kernel/vga.h>
#include <stdint.h>

#define MULTIBOOT2_MAGIC 0x36d76289

extern void setup_framebuffer(uint32_t, uint32_t, uint8_t, uint64_t);
extern void handle_memory_map(struct multiboot_mmap_entry *);

void kernel_init(uint32_t magic, uint32_t *mb_info_addr) {
  if (magic != MULTIBOOT2_MAGIC)
    return;

  uint8_t *ptr = (uint8_t *)mb_info_addr + 8;

  while (1) {
    struct multiboot_tag *tag = (struct multiboot_tag *)ptr;
    if (tag->type == 0 && tag->size == 8)
      break;

    switch (tag->type) {
    case 6: { // Memory map
      struct multiboot_tag_mmap *mmap = (struct multiboot_tag_mmap *)tag;
      uint32_t count = (mmap->size - 16) / mmap->entry_size;
      for (uint32_t i = 0; i < count; i++) {
        handle_memory_map(&mmap->entries[i]);
      }
      break;
    }
    case 8: { // Framebuffer
      struct multiboot_tag_framebuffer *fb =
          (struct multiboot_tag_framebuffer *)tag;
      setup_framebuffer(fb->width, fb->height, fb->bpp, fb->addr);
      break;
    }
    }
    ptr += (tag->size + 7) & ~7;
  }

  // Draw test pattern
  for (int y = 0; y < 100; y++)
    for (int x = 0; x < 100; x++)
      put_pixel(x, y, 0x00FF0000); // Red square
}
