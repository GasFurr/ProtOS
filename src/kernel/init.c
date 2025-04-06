//
// Kernel initialization.
//
#include "gdt.h"
#include "graphic.h"
#include "idt.h"
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
  clear_screen(BLACK); // black background
  init_font();

  set_cursor_pos(0, 0);
  set_text_color(PROTOS_BLUE, BLACK);

  serial_puts("Initializing timer...\n");
  KOutput("[GDT] Initialized\n[PIC] Remap completed\n[OUT] Serial out "
          "initialized \n[IDT] Initialized \n [MB2] Tags parsed\n");
  timer_init(1000);

  serial_puts("PIT configured for ");
  serial_puthex(1193180 / (PIT_FREQUENCY / 1000)); // Should show 1000
  serial_puts("Hz\n");
  KOutput("[TIMER] PIT Configured");

  // 1. Check initial tick count
  serial_puts("Initial ticks: ");
  serial_puthex(timer_ticks());
  serial_puts("\n");

  // 2. Simple count-up test
  for (int i = 0; i < 5; i++) {
    uint32_t start = timer_ticks();
    serial_puts("Waiting 100ms... ");
    sleep(100); // Use your sleep function
    uint32_t elapsed = timer_ticks() - start;

    serial_puts("Elapsed: ");
    serial_puthex(elapsed);
    serial_puts(" ticks\n");
  }
  KOutput("\n[TIMER] Working");
  sleep(10);
  KOutput("\nCalling KMain...\n");
  serial_puts("Calling KMain... \n ------ \n");
  KMain();

  hcf();
}
