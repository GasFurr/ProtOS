// text.h
#ifndef TEXT_H
#define TEXT_H

#include <stdint.h>

// Add these extern declarations
extern uint32_t char_width;
extern uint32_t char_height;
extern uint32_t font_scale;
extern uint32_t font_spacing;

void init_font(void);
void set_text_color(uint32_t fg, uint32_t bg);
void set_cursor_pos(uint32_t x, uint32_t y);
void draw_char(unsigned char c);
void print_text(const char *str);

#endif
