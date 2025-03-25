#include <kernel/multiboot_tags.h>
#define MULTIBOOT2_MAGIC 0x36d76289

void parse_tags(uint32_t magic, uint32_t *mb_info_addr) {
    if (magic != MULTIBOOT2_MAGIC) return;

    uint8_t *ptr = (uint8_t *)mb_info_addr + 8;
    
    while (1) {
        struct multiboot_tag *tag = (struct multiboot_tag *)ptr;
        if (tag->type == 0 && tag->size == 8) break;

        switch (tag->type) {
            case 6: {
                struct multiboot_tag_mmap *mmap = (struct multiboot_tag_mmap *)tag;
                uint32_t count = (mmap->size - 16) / mmap->entry_size;
                for (uint32_t i = 0; i < count; i++) {
                    handle_memory_map(&mmap->entries[i]);
                }
                break;
            }
            
            case 8: {
                struct multiboot_tag_framebuffer *fb = (struct multiboot_tag_framebuffer *)tag;
                setup_framebuffer(fb->width, fb->height, fb->bpp, fb->addr);
                break;
            }
            
            case 15: {
                struct multiboot_tag_uefi *uefi = (struct multiboot_tag_uefi *)tag;
                handle_uefi_system_table(uefi->pointer);
                break;
            }
        }
        
        ptr += (tag->size + 7) & ~7;
    }
}

void kernel_init(){
  parse_tags();
  kernel_entry();
}
