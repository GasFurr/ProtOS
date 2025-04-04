// graphics.c - Framebuffer Graphics Implementation
// graphics.c - Framebuffer Graphics Implementation
#include "graphic.h"
#include "mb2tags.h" // Use the updated header
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Internal state
static struct multiboot_tag_framebuffer *fb = NULL;
static uint32_t bytes_per_pixel = 0;

// returns absoulte value
static inline int32_t iabs(int32_t x) { return x < 0 ? -x : x; }

/**
 * Convert 24-bit RGB color to native framebuffer format
 * @param rgb 24-bit color in 0xRRGGBB format
 * @return Native color format, 0 if unsupported mode
 */
static uint32_t convert_color(uint32_t rgb) {
  // Extract components from 24-bit RGB
  const uint8_t r = (rgb >> 16) & 0xFF; // Red: 0xFF0000 -> 0xFF
  const uint8_t g = (rgb >> 8) & 0xFF;  // Green: 0x00FF00 -> 0xFF
  const uint8_t b = rgb & 0xFF;         // Blue: 0x0000FF -> 0xFF

  // For ARGB/XRGB format (most common)
  return 0xFF000000 | // Alpha channel (opaque)
         (r << 16) |  // Red in bits 16-23
         (g << 8) |   // Green in bits 8-15
         b;           // Blue in bits 0-7
}

/**
 * Initialize graphics subsystem with framebuffer information
 * @param fb_tag Valid framebuffer tag from Multiboot2
 */
void graphics_init(struct multiboot_tag_framebuffer *fb_tag) {
  if (!fb_tag || fb_tag->framebuffer_type != FB_TYPE_DIRECTRGB) {
    return; // Only support direct RGB modes
  }

  fb = fb_tag;
  bytes_per_pixel = fb->framebuffer_bpp / 8;
}

/**
 * Draw a single pixel to the framebuffer
 * @param x X coordinate (0 = leftmost)
 * @param y Y coordinate (0 = topmost)
 * @param color 24-bit RGB color (0xRRGGBB)
 */
void draw_pixel(uint32_t x, uint32_t y, uint32_t color) {
  if (!fb || x >= fb->framebuffer_width || y >= fb->framebuffer_height) {
    return;
  }

  uint8_t *pixel = (uint8_t *)(uintptr_t)fb->framebuffer_addr +
                   y * fb->framebuffer_pitch + x * bytes_per_pixel;

  *(uint32_t *)pixel = convert_color(color);
}

/**
 * Clear screen with specified color
 * @param color 24-bit RGB color (0xRRGGBB)
 */
void clear_screen(uint32_t color) {
  if (!fb)
    return;

  uint8_t *fb_mem = (uint8_t *)(uintptr_t)fb->framebuffer_addr;
  const uint32_t native_color = convert_color(color);
  const uint32_t bytes_per_pixel = 4;

  for (uint32_t y = 0; y < fb->framebuffer_height; y++) {
    uint32_t *row = (uint32_t *)(fb_mem + y * fb->framebuffer_pitch);
    for (uint32_t x = 0; x < fb->framebuffer_width; x++) {
      row[x] = native_color;
    }
  }
}

/**
 * Draw a rectangle
 * @param x Left position
 * @param y Top position
 * @param width Rectangle width
 * @param height Rectangle height
 * @param color 24-bit RGB color
 * @param filled 0 = outline, 1 = filled
 */
void draw_rect(uint32_t x, uint32_t y, uint32_t width, uint32_t height,
               uint32_t color, int filled) {
  if (!fb)
    return;

  // Clamp to screen boundaries
  const uint32_t max_x =
      x + width < fb->framebuffer_width ? x + width : fb->framebuffer_width;
  const uint32_t max_y =
      y + height < fb->framebuffer_height ? y + height : fb->framebuffer_height;

  for (uint32_t curr_y = y; curr_y < max_y; curr_y++) {
    for (uint32_t curr_x = x; curr_x < max_x; curr_x++) {
      if (!filled && curr_x > x && curr_x < max_x - 1 && curr_y > y &&
          curr_y < max_y - 1) {
        continue; // Skip interior for outline
      }
      draw_pixel(curr_x, curr_y, color);
    }
  }
}

/**
 * Draw line using Bresenham's algorithm with variable thickness
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

  // Calculate line direction properties
  int32_t abs_dx = iabs(x1 - x0);
  int32_t abs_dy = iabs(y1 - y0);
  bool is_steep = abs_dy > abs_dx; // Determine primary axis

  // Bresenham algorithm setup
  int32_t dx = iabs(x1 - x0);
  int32_t dy = -iabs(y1 - y0);
  int32_t sx = x0 < x1 ? 1 : -1;
  int32_t sy = y0 < y1 ? 1 : -1;
  int32_t err = dx + dy;

  while (1) {
    // Only draw if coordinates are within bounds
    if (x0 >= 0 && (uint32_t)x0 < fb->framebuffer_width && y0 >= 0 &&
        (uint32_t)y0 < fb->framebuffer_height) {

      // Calculate thickness offsets
      int start_offset = (1 - thickness) / 2;
      int end_offset = thickness / 2;

      // Draw perpendicular to the primary axis
      if (is_steep) {
        // Horizontal spread for steep lines (vertical-dominant)
        for (int i = start_offset; i <= end_offset; ++i) {
          int32_t current_x = x0 + i;
          if (current_x >= 0 && (uint32_t)current_x < fb->framebuffer_width) {
            draw_pixel(current_x, y0, color);
          }
        }
      } else {
        // Vertical spread for shallow lines (horizontal-dominant)
        for (int i = start_offset; i <= end_offset; ++i) {
          int32_t current_y = y0 + i;
          if (current_y >= 0 && (uint32_t)current_y < fb->framebuffer_height) {
            draw_pixel(x0, current_y, color);
          }
        }
      }
    }

    // Exit condition: reached end point
    if (x0 == x1 && y0 == y1)
      break;

    // Bresenham error adjustment
    int32_t e2 = 2 * err;
    if (e2 >= dy) {
      err += dy;
      x0 += sx;
    }
    if (e2 <= dx) {
      err += dx;
      y0 += sy;
    }
  }
}
