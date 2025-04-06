// graphics.c - Optimized Framebuffer Graphics
#include "graphic.h"
#include "mb2tags.h"
#include "serial.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Internal state
struct multiboot_tag_framebuffer *fb = NULL;
uint32_t *fb_mem = NULL;
uint32_t fb_pitch = 0;

// Fast absolute value
#define iabs(x) ((x) < 0 ? -(x) : (x))

/**
 * Precompute native color format
 * @param rgb 24-bit color in 0xRRGGBB format
 * @return Native color format, 0 if unsupported mode
 */
static inline uint32_t convert_color(uint32_t rgb) {
  // Compile-time constant mask calculations
  const uint32_t r = (rgb >> 16) & 0xFF;
  const uint32_t g = (rgb >> 8) & 0xFF;
  const uint32_t b = rgb & 0xFF;
  return 0xFF000000 | (r << 16) | (g << 8) | b;
}

/**
 * Initialize graphics subsystem
 */
void graphics_init(struct multiboot_tag_framebuffer *fb_tag) {
  if (!fb_tag || fb_tag->framebuffer_type != FB_TYPE_DIRECTRGB)
    return;

  fb = fb_tag;
  fb_mem = (uint32_t *)(uintptr_t)fb->framebuffer_addr;
  fb_pitch = fb->framebuffer_pitch / sizeof(uint32_t);
  serial_puts("Graphics initialized!\n");
}

/**
 * Bulk pixel writer (aligned 32-bit writes)
 */
static inline void bulk_write(uint32_t *dest, uint32_t color, size_t count) {
  while (count--)
    *dest++ = color;
}

void draw_pixel(uint32_t x, uint32_t y, uint32_t color) {
  if (!fb || x >= fb->framebuffer_width || y >= fb->framebuffer_height)
    return;

  // Calculate pixel position
  uint8_t *pixel = (uint8_t *)(uintptr_t)fb->framebuffer_addr +
                   y * fb->framebuffer_pitch + x * (fb->framebuffer_bpp / 8);

  // Write color value
  *(uint32_t *)pixel = convert_color(color);
}

/**
 * Optimized screen clear
 */
void clear_graphics(uint32_t color) {
  if (!fb)
    return;

  const uint32_t native = convert_color(color);
  const size_t stride = fb_pitch;
  const size_t total = fb->framebuffer_height * stride;

  // Fill memory in large chunks
  for (size_t i = 0; i < total; i += stride) {
    bulk_write(fb_mem + i, native, fb->framebuffer_width);
  }
}

/**
 * Fast rectangle drawing
 */
void draw_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color,
               int filled) {
  if (!fb || x >= fb->framebuffer_width || y >= fb->framebuffer_height)
    return;

  // Precompute native color once
  const uint32_t native = convert_color(color);
  const uint32_t max_x =
      (x + w) < fb->framebuffer_width ? x + w : fb->framebuffer_width;
  const uint32_t max_y =
      (y + h) < fb->framebuffer_height ? y + h : fb->framebuffer_height;

  if (filled) {
    // Optimized filled rectangle
    for (uint32_t cy = y; cy < max_y; cy++) {
      uint32_t *line = fb_mem + cy * fb_pitch + x;
      bulk_write(line, native, max_x - x);
    }
  } else {
    // Optimized outline using horizontal lines
    bulk_write(fb_mem + y * fb_pitch + x, native, max_x - x); // Top
    bulk_write(fb_mem + (max_y - 1) * fb_pitch + x, native,
               max_x - x); // Bottom

    // Vertical lines
    for (uint32_t cy = y; cy < max_y; cy++) {
      fb_mem[cy * fb_pitch + x] = native;           // Left
      fb_mem[cy * fb_pitch + (max_x - 1)] = native; // Right
    }
  }
}

/**
 * Draw line using Bresenham's algorithm with proper thickness
 * @param x0 Start X
 * @param y0 Start Y
 * @param x1 End X
 * @param y1 End Y
 * @param color 24-bit RGB color
 * @param thickness Line thickness in pixels (minimum 1)
 */
void draw_line(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t color,
               int thickness) {
  if (!fb || thickness < 1)
    return;

  const uint32_t native = convert_color(color);
  const bool steep = iabs(y1 - y0) > iabs(x1 - x0);
  int32_t dx = iabs(x1 - x0);
  int32_t dy = iabs(y1 - y0);
  int32_t err = dx - dy;
  int32_t sx = x0 < x1 ? 1 : -1;
  int32_t sy = y0 < y1 ? 1 : -1;

  // Proper thickness calculation for both even/odd values
  const int t_half = thickness / 2;
  const int t_start = -t_half;
  const int t_end = t_half + (thickness % 2) - 1;

  while (1) {
    // Draw thickness perpendicular to line direction
    for (int t = t_start; t <= t_end; t++) {
      int32_t px, py;

      if (steep) {
        // Vertical line - spread horizontally
        px = x0 + t;
        py = y0;
      } else {
        // Horizontal line - spread vertically
        px = x0;
        py = y0 + t;
      }

      // Boundary checks
      if (px >= 0 && (uint32_t)px < fb->framebuffer_width && py >= 0 &&
          (uint32_t)py < fb->framebuffer_height) {
        fb_mem[py * fb_pitch + px] = native;
      }
    }

    if (x0 == x1 && y0 == y1)
      break;

    int32_t e2 = 2 * err;
    if (e2 > -dy) {
      err -= dy;
      x0 += sx;
    }
    if (e2 < dx) {
      err += dx;
      y0 += sy;
    }
  }
  serial_puts("Line drawn\n");
}

uint32_t get_pixel(uint32_t x, uint32_t y) {
  if (!fb || x >= fb->framebuffer_width || y >= fb->framebuffer_height)
    return 0;

  // Direct access using precomputed pitch
  return fb_mem[y * fb_pitch + x];
}
