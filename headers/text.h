#ifndef TEXT_H
#define TEXT_H

#include <stddef.h>
#include <stdint.h>

// Text color management
void set_text_color(uint32_t text, uint32_t bg);
void set_background_color(uint32_t color);

// Cursor control
void set_cursor_pos(uint32_t x, uint32_t y);
void get_cursor_pos(uint32_t *x, uint32_t *y);
void advance_cursor();

// Text output
void KOutput(const char *str);
void KOutput_color(const char *str, uint32_t fg, uint32_t bg);
void draw_char(unsigned char c);
void erase_char();

// Screen management
void scroll_screen(uint32_t lines);
void clear_screen();

// Font initialization
void init_font();
void set_font_scale(uint32_t scale);

void KOutput_center(const char *str);

#endif
