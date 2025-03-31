#include <stdint.h>

static uint32_t *framebuffer = 0;
static uint32_t fb_width = 0, fb_height = 0;

void setup_framebuffer(uint32_t width, uint32_t height, uint8_t bpp,
                       uint64_t addr) {
  framebuffer = (uint32_t *)(uintptr_t)addr;
  fb_width = width;
  fb_height = height;
}

void put_pixel(uint32_t x, uint32_t y, uint32_t color) {
  if (x >= fb_width || y >= fb_height)
    return;
  framebuffer[y * fb_width + x] = color;
}
