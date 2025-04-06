#include "text.h"
#include "font.h"
#include "graphic.h"
#include "mb2tags.h"
#include "serial.h"
#include "string.h"
#include "time.h"

// Text state
static uint32_t text_color = 0xFFFFFF;
static uint32_t bg_color = 0x000000;
static uint32_t cursor_x = 0;
static uint32_t cursor_y = 0;
static uint32_t char_width = 8;
static uint32_t char_height = 16;
static uint32_t font_scale = 1;
static uint32_t char_spacing = 1;
static uint32_t line_spacing = 2;
static uint32_t current_bg_color = 0x000000;

// Font data
static const struct psf2_header *font_header;
static const uint8_t *font_glyphs;

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

  serial_puts("Font initialized!\n");
  // draw_rect(150, 0, 20, 20, 0x00FF00, 1); // Green success
}

void set_background_color(uint32_t color) { current_bg_color = color; }

void set_font_scale(uint32_t scale) {
  font_scale = (scale < 1) ? 1 : (scale > 4 ? 4 : scale);
}

void set_text_color(uint32_t fg, uint32_t bg) {
  text_color = fg;
  current_bg_color = bg; // Update background color
}
void set_cursor_pos(uint32_t x, uint32_t y) {
  cursor_x = x * (char_width + char_spacing) * font_scale;
  cursor_y = y * (char_height + line_spacing) * font_scale;

  // Clamp to screen bounds
  if (cursor_x >= fb->framebuffer_width)
    cursor_x = fb->framebuffer_width - (char_width * font_scale);
  if (cursor_y >= fb->framebuffer_height)
    cursor_y = fb->framebuffer_height - (char_height * font_scale);
}

static uint32_t calculate_text_width(const char *str) {
  size_t len = strlen(str);
  if (len == 0)
    return 0;
  return len * (char_width + char_spacing) * font_scale -
         char_spacing * font_scale;
}

void advance_cursor() {
  cursor_x += (char_width + char_spacing) * font_scale;

  // Handle line wrap
  if (cursor_x + (char_width * font_scale) > fb->framebuffer_width) {
    cursor_x = 0;
    cursor_y += (char_height + line_spacing) * font_scale;

    // Handle screen scroll
    if (cursor_y + (char_height * font_scale) > fb->framebuffer_height) {
      scroll_screen(1);
      cursor_y -= (char_height + line_spacing) * font_scale;
    }
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

void erase_char() {
  if (cursor_x == 0 && cursor_y == 0)
    return;

  // Move back one character
  if (cursor_x == 0) {
    if (cursor_y >= (char_height + line_spacing) * font_scale) {
      cursor_y -= (char_height + line_spacing) * font_scale;
    }
    cursor_x = fb->framebuffer_width - (char_width * font_scale);
  } else {
    cursor_x -= (char_width + char_spacing) * font_scale;
  }

  // Clear character area
  draw_rect(cursor_x, cursor_y, char_width * font_scale,
            char_height * font_scale, bg_color, 1);
}

void scroll_screen(uint32_t lines) {
  uint32_t scroll_pixels = lines * (char_height + line_spacing) * font_scale;
  uint32_t bytes_per_line = fb->framebuffer_pitch;

  // Scroll framebuffer contents
  uint8_t *dst = (uint8_t *)fb->framebuffer_addr;
  uint8_t *src = dst + scroll_pixels * bytes_per_line;
  size_t size = (fb->framebuffer_height - scroll_pixels) * bytes_per_line;
  memmove(dst, src, size);

  // Clear new area with current background color
  uint32_t *fb_end = (uint32_t *)(dst + size);
  size_t pixels_to_clear = (scroll_pixels * bytes_per_line) / sizeof(uint32_t);

  for (size_t i = 0; i < pixels_to_clear; i++) {
    fb_end[i] = current_bg_color;
  }
}

void KOutput(const char *str) {
  for (size_t i = 0; str[i]; i++) {
    switch (str[i]) {
    case '\n':
      cursor_x = 0;
      cursor_y += (char_height + line_spacing) * font_scale;
      if (cursor_y + (char_height * font_scale) > fb->framebuffer_height) {
        scroll_screen(1);
        cursor_y -= (char_height + line_spacing) * font_scale;
      }
      break;
    case '\t':
      for (int t = 0; t < 4; t++)
        draw_char(' ');
      break;
    case '\b':
      erase_char();
      break;
    default:
      draw_char(str[i]);
      break;
    }
  }
}

// This shet not working, just hardcoded it to (sometimes) work.
// I hope in 4.0.0 i will fix it.
void KOutput_center(const char *str) {
  uint32_t saved_x = cursor_x;
  uint32_t text_width = calculate_text_width(str);
  cursor_x = (fb->framebuffer_width - text_width) / 3;
  KOutput(str);
  cursor_x = saved_x;
}

void KOutput_left(const char *str) {
  uint32_t saved_x = cursor_x;
  cursor_x = 0;
  KOutput(str);
  cursor_x = saved_x;
}

void KOutput_right(const char *str) {
  uint32_t saved_x = cursor_x;
  uint32_t text_width = calculate_text_width(str);
  cursor_x = fb->framebuffer_width - text_width;
  KOutput(str);
  cursor_x = saved_x;
}

void KOutput_color(const char *str, uint32_t fg, uint32_t bg) {
  uint32_t saved_fg = text_color;
  uint32_t saved_bg = bg_color;
  set_text_color(fg, bg);
  KOutput(str);
  set_text_color(saved_fg, saved_bg);
}

void clear_screen() {
  if (!fb)
    return;

  const size_t total_pixels = fb->framebuffer_width * fb->framebuffer_height;
  for (size_t i = 0; i < total_pixels; i++) {
    fb_mem[i] = current_bg_color;
  }

  cursor_x = 0;
  cursor_y = 0;
}

void clear_line(uint32_t y) {
  uint32_t line_height = (char_height + line_spacing) * font_scale;
  draw_rect(0, y * line_height, fb->framebuffer_width, line_height,
            current_bg_color, 1);
}
