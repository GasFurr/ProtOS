// text.c
#include "text.h"
#include "font.h" // Contains PSF2 header
#include "graphic.h"
#include "mb2tags.h"
#include <stddef.h>

static uint32_t text_color = 0xFFFFFF;
static uint32_t bg_color = 0x000000;
static uint32_t cursor_x = 0;
static uint32_t cursor_y = 0;
uint32_t char_width = 8;
uint32_t char_height = 16;
uint32_t font_scale = 2;
uint32_t char_spacing = 2;
uint32_t line_spacing = 2;

static const struct psf2_header *font_header;
static const uint8_t *font_glyphs;

static int memcmp(const void *s1, const void *s2, size_t n) {
  const unsigned char *p1 = s1, *p2 = s2;
  for (size_t i = 0; i < n; i++) {
    if (p1[i] != p2[i])
      return p1[i] - p2[i];
  }
  return 0;
}

void init_font(void) {
  font_header = (const struct psf2_header *)&_binary_font_psf_start;

  // Manually check PSF2 magic: 0x72 0xb5 0x4a 0x86
  const uint8_t expected_magic[4] = {0x72, 0xb5, 0x4a, 0x86};
  uint8_t actual_magic[4];

  // Manually copy first 4 bytes
  for (int i = 0; i < 4; i++) {
    actual_magic[i] = ((const uint8_t *)font_header)[i];
  }

  if (memcmp(actual_magic, expected_magic, 4) != 0) {
    draw_rect(0, 0, 20, 20, 0xFF0000, 1); // Red error
    return;
  }

  char_width = font_header->width;
  char_height = font_header->height;
  font_glyphs = (const uint8_t *)font_header + font_header->headersize;

  // draw_rect(150, 0, 20, 20, 0x00FF00, 1); // Green success
}

void set_text_color(uint32_t fg, uint32_t bg) {
  text_color = fg;
  bg_color = bg;
}

void set_cursor_pos(uint32_t x, uint32_t y) {
  cursor_x = x * char_width * font_scale;  // Add scaling
  cursor_y = y * char_height * font_scale; // Add scaling
}

// Add missing advance_cursor function
static void advance_cursor(void) {
  cursor_x += char_width * font_scale;

  // Check against scaled width
  if (cursor_x + (char_width * font_scale) > fb->framebuffer_width) {
    cursor_x = 0;
    cursor_y += char_height * font_scale; // Scaled line height
  }
}

void draw_char(unsigned char c) {
  if (c < 32 || c > 127)
    return;

  // Calculate glyph offset using PSF2 header data
  const uint8_t *glyph = font_glyphs + (c * font_header->bytes_per_glyph);

  // Calculate bytes per row (ceil(width/8))
  const uint32_t bytes_per_row = (font_header->width + 7) / 8;

  for (uint32_t y = 0; y < font_header->height; y++) {
    for (uint32_t byte = 0; byte < bytes_per_row; byte++) {
      uint8_t glyph_byte = glyph[y * bytes_per_row + byte];

      for (uint32_t bit = 0; bit < 8; bit++) {
        uint32_t x = byte * 8 + bit;
        if (x >= font_header->width)
          break;

        if (glyph_byte & (0x80 >> bit)) {
          // Draw scaled pixel
          for (uint32_t sy = 0; sy < font_scale; sy++) {
            for (uint32_t sx = 0; sx < font_scale; sx++) {
              draw_pixel(cursor_x + x * font_scale + sx,
                         cursor_y + y * font_scale + sy, text_color);
            }
          }
        }
      }
    }
  }
  cursor_x += char_spacing * font_scale;
  advance_cursor();
}

void KOutput(const char *str) {
  for (size_t i = 0; str[i]; i++) {
    if (str[i] == '\n') {
      cursor_x = 0;
      cursor_y += (char_height + line_spacing) * font_scale;
    } else if (str[i] == '\t') {
      cursor_x += (char_width + char_spacing) * font_scale * 4;
    } else {
      draw_char(str[i]);
    }

    // Simple word wrap
    if (cursor_x + (char_width * font_scale) > fb->framebuffer_width) {
      cursor_x = 0;
      cursor_y += (char_height + line_spacing) * font_scale;
    }
  }
}
