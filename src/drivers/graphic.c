// graphics.c - Framebuffer Graphics Implementation
// graphics.c - Framebuffer Graphics Implementation
#include "graphic.h"
#include "mb2tags.h" // Use the updated header
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
  if (!fb || fb->framebuffer_type != FB_TYPE_DIRECTRGB) {
    return 0;
  }

  // Extract color components
  const uint8_t red = (rgb >> 16) & 0xFF;
  const uint8_t green = (rgb >> 8) & 0xFF;
  const uint8_t blue = rgb & 0xFF;

  // Construct native color value
  return (red << fb->direct_rgb.red_position) |
         (green << fb->direct_rgb.green_position) |
         (blue << fb->direct_rgb.blue_position);
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

  const uint32_t native_color = convert_color(color);
  uint32_t *fb_mem = (uint32_t *)(uintptr_t)fb->framebuffer_addr;
  const uint32_t pixel_count = fb->framebuffer_width * fb->framebuffer_height;

  for (uint32_t i = 0; i < pixel_count; i++) {
    fb_mem[i] = native_color;
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
 * Draw line using Bresenham's algorithm
 * @param x0 Start X
 * @param y0 Start Y
 * @param x1 End X
 * @param y1 End Y
 * @param color 24-bit RGB color
 */
void draw_line(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t color) {
  if (!fb)
    return;

  int32_t dx = iabs(x1 - x0);
  int32_t dy = -iabs(y1 - y0);
  int32_t sx = x0 < x1 ? 1 : -1;
  int32_t sy = y0 < y1 ? 1 : -1;
  int32_t err = dx + dy;

  while (1) {
    if (x0 >= 0 && (uint32_t)x0 < fb->framebuffer_width && y0 >= 0 &&
        (uint32_t)y0 < fb->framebuffer_height) {
      draw_pixel(x0, y0, color);
    }

    if (x0 == x1 && y0 == y1)
      break;

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
