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
  clear_screen(PROTOS_BLACK); // Protos-black background
  // Test pattern
  draw_rect(10, 10, 100, 100, RED, 1);    // Red
  draw_rect(120, 10, 100, 100, GREEN, 1); // Green
  draw_rect(230, 10, 100, 100, BLUE, 1);  // Blue
  draw_rect(340, 10, 100, 100, WHITE, 1); // White
  // Protos-Blue line
  draw_line(0, 125, fb_tag->framebuffer_width - 1, 125, PROTOS_BLUE, 10);
  while (1)
    __asm__("hlt");
}
