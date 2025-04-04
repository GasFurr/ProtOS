//
// Main kernel file. Where everything works together.
//
#include "graphic.h"
#include "mb2tags.h"

void KInit(uint32_t magic, uint32_t *mb2_info) {
  (void)magic; // Explicitly mark as unused

  struct multiboot_tag_framebuffer *fb_tag = mb2_get_fb_tag(mb2_info);
  if (!fb_tag)
    return;

  // Initialize graphics subsystem
  graphics_init(fb_tag);

  // Basic framebuffer test
  clear_screen(0x000000); // Black background

  // Red rectangle (100x50 pixels at position 50,50)
  draw_rect(50, 50, 100, 50, 0xFF0000, 1);

  // Green diagonal line from top-left to bottom-right
  draw_line(0, 0, fb_tag->framebuffer_width - 1, fb_tag->framebuffer_height - 1,
            0x00FF00);

  // Halt indefinitely
  while (1)
    __asm__("hlt");
}
