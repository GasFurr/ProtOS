// graphics.h - Basic Framebuffer Graphics Library
#pragma once
#include "mb2tags.h"
#include <stdint.h>

#define SWAP(a, b)                                                             \
  do {                                                                         \
    typeof(a) temp = a;                                                        \
    a = b;                                                                     \
    b = temp;                                                                  \
  } while (0)
#define PROTOS_BLACK 0x042F40
#define PROTOS_BLUE 0x2499C5
#define BLACK 0x000000
#define WHITE 0xFFFFFF
#define RED 0xFF0000
#define GREEN 0x00FF00
#define BLUE 0x0000FF

// Forward-declare the struct (no redefinition)

/**
 * Initialize graphics library with framebuffer information.
 * Must be called before using any drawing functions.
 *
 * @param fb Pointer to the framebuffer info struct (from Multiboot2)
 */
void graphics_init(struct multiboot_tag_framebuffer *fb_tag);

/**
 * Draw a single pixel to the framebuffer.
 *
 * @param x     X-coordinate (0 = leftmost)
 * @param y     Y-coordinate (0 = topmost)
 * @param color 32-bit color value (0xRRGGBB format)
 */
void draw_pixel(uint32_t x, uint32_t y, uint32_t color);

/**
 * Fill the entire screen with a solid color.
 *
 * @param color 32-bit color value (0xRRGGBB format)
 */
void clear_screen(uint32_t color);

/**
 * Draw a rectangle (filled or outline).
 *
 * @param x      X-coordinate of top-left corner
 * @param y      Y-coordinate of top-left corner
 * @param width  Width of the rectangle
 * @param height Height of the rectangle
 * @param color  Fill color (0xRRGGBB)
 * @param filled 0 = outline, 1 = filled
 */
void draw_rect(uint32_t x, uint32_t y, uint32_t width, uint32_t height,
               uint32_t color, int filled);

/**
 * Draw a line between two points using Bresenham's algorithm.
 *
 * @param x0    Start X-coordinate
 * @param y0    Start Y-coordinate
 * @param x1    End X-coordinate
 * @param y1    End Y-coordinate
 * @param color Line color (0xRRGGBB)
 * @param thickness Line thickness.
 */
void draw_line(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t color,
               int thickness);
