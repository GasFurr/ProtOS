//
// Kernel initialization.
//
#include "gdt.h"
#include "graphic.h"
#include "idt.h"
#include "kb.h"
#include "kernel.h"
#include "mb2tags.h"
#include "pic.h"
#include "serial.h"
#include "text.h"
#include "time.h"

void KInit(uint32_t magic, uint32_t *mb2_info) {
  (void)magic;
  gdt_init();
  pic_remap();
  idt_init();
  keyboard_init();
  asm volatile("sti");
  serial_init();

  struct multiboot_tag_framebuffer *fb_tag = mb2_get_fb_tag(mb2_info);
  if (!fb_tag) {
    // Draw fatal error pattern
    for (uint32_t y = 0; y < 50; y++)
      for (uint32_t x = 0; x < 50; x++)
        draw_pixel(x, y, 0xFF0000);
    serial_puts("FATAL MULTIBOOT ERROR\n");
    while (1)
      ;
  }

  serial_puts("Multiboot tags parsed\n");
  graphics_init(fb_tag);
  init_font();

  set_cursor_pos(0, 0);
  set_background_color(BLACK);
  set_text_color(PROTOS_BLUE, BLACK);
  clear_screen(); // black background
  timer_init(1000);
  KMain();

  hcf();
}
