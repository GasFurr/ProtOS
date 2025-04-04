//
// Main kernel file. Where everything works together.
//
#include "graphic.h"
#include "mb2tags.h"
#include "text.h"

void KInit(uint32_t magic, uint32_t *mb2_info) {
  (void)magic;

  struct multiboot_tag_framebuffer *fb_tag = mb2_get_fb_tag(mb2_info);
  if (!fb_tag) {
    // Draw fatal error pattern
    for (uint32_t y = 0; y < 50; y++)
      for (uint32_t x = 0; x < 50; x++)
        draw_pixel(x, y, 0xFF0000);
    while (1)
      ;
  }

  graphics_init(fb_tag);
  clear_screen(BLACK); // black background
  init_font();
  set_cursor_pos(0, 0);
  set_text_color(PROTOS_BLUE, BLACK);
  KOutput("Test pattern:");
  // Basic framebuffer test
  // Test pattern
  draw_rect(100, 40, 100, 100, RED, 1);   // Red
  draw_rect(210, 40, 100, 100, GREEN, 1); // Green
  draw_rect(320, 40, 100, 100, BLUE, 1);  // Blue
  draw_rect(430, 40, 100, 100, WHITE, 1); // White
  // Protos-Blue line
  draw_line(0, 155, fb_tag->framebuffer_width - 1, 155, PROTOS_BLUE, 10);

  // Draw debug grid

  set_cursor_pos(0, 5);
  KOutput(" ProtOS-Alpha 0.1.8\n");
  KOutput(" Initialization Finished!\n");

  while (1)
    __asm__("hlt");
}
