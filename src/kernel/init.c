//
// Kernel initialization.
//
#include "gdt.h"
#include "graphic.h"
#include "idt.h"
#include "kernel.h"
#include "mb2tags.h"
#include "serial.h"
#include "text.h"

void test_gdt_protection() {
  // First validate GDT works for normal access
  volatile uint32_t *valid_ptr = (uint32_t *)0x00100000;
  *valid_ptr = 0xDEADBEEF;

  // Then test invalid execution
  void (*bad_jump)() = (void (*)())valid_ptr;
  serial_puts("Testing GDT protection...\n");
  bad_jump(); // Should trigger GPF

  // This should never be reached if protection works
  serial_puts("GDT PROTECTION FAILED!\n");
}

void KInit(uint32_t magic, uint32_t *mb2_info) {
  (void)magic;
  gdt_init();
  idt_init();
  asm volatile("" ::: "memory");
  serial_init();
  gdt_debug();
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
  clear_screen(BLACK); // black background
  init_font();

  set_cursor_pos(0, 0);
  set_text_color(PROTOS_BLUE, BLACK);
  KOutput("GDT Loaded: ");
  KOutput(gdt[1].access == 0x9A ? "VALID" : "CORRUPT\n");
  KOutput("Testing GDT PROTECTION \n");
  /*
  KOutput("Test pattern:");
  // Basic framebuffer test
  // Test pattern
  draw_rect(100, 40, 100, 100, RED, 1);   // Red
  draw_rect(210, 40, 100, 100, GREEN, 1); // Green
  draw_rect(320, 40, 100, 100, BLUE, 1);  // Blue
  draw_rect(430, 40, 100, 100, WHITE, 1); // White
  // Protos-Blue line
  draw_line(0, 155, fb_tag->framebuffer_width - 1, 155, PROTOS_BLUE, 10);

  set_cursor_pos(0, 5);
  KOutput(" ProtOS-Alpha 0.1.9\n");
  KOutput(" Initialization Finished!\n");
  serial_puts(" ProtOS-Alpha 0.1.9");
  */
  serial_puts("Calling KMain... \n ------ \n");
  KMain();

  hcf();
}
