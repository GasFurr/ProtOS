Here's comprehensive documentation for your text driver:

---

# Text Driver Documentation (`text.c`/`text.h`)

## Overview

Provides framebuffer-based text rendering using bitmap fonts. Supports:

- 8x16 pixel characters (PSF1 format)
- Custom colors (24-bit RGB)
- Cursor positioning
- Basic control characters (\n, \t)
- Multiple font support

## Dependencies

- Graphic driver (`graphic.c`)
- PSF font file (compiled into kernel)
- Multiboot2 framebuffer initialization

---

## Setup Guide

### 1. Font Preparation

1. Place PSF font file in `src/fonts/` (e.g., `ter-powerline-v28b.psf`)
2. Update Makefile to compile font:

```makefile
# In Makefile
FontDir = src/fonts
Font = $(FontDir)/ter-powerline-v28b.psf
FontObject = $(BuildDir)/font.o
```

### 2. Initialization Sequence

```c
// In kernel main()
graphics_init(fb_tag);  // Initialize framebuffer first
init_font();            // Then initialize font
set_text_color(0xFFFFFF, 0x000000); // White on black
```

---

## API Reference

### 1. `void init_font(void)`

Initializes font system using embedded PSF font.

**Must be called after** `graphics_init()`!

### 2. `void set_text_color(uint32_t fg, uint32_t bg)`

Sets foreground and background colors.

| Parameter | Format     | Description          |
|-----------|------------|----------------------|
| `fg`      | 0xRRGGBB   | Text color (24-bit)  |
| `bg`      | 0xRRGGBB   | Background color     |

**Example:**

```c
set_text_color(0xFF0000, 0x00FF00); // Red text on green background
```

### 3. `void set_cursor_pos(uint32_t x, uint32_t y)`

Sets text cursor position in **character coordinates** (not pixels).

| Parameter | Range                      |
|-----------|----------------------------|
| `x`       | 0 to (screen_width/8)-1    |
| `y`       | 0 to (screen_height/16)-1  |

**Example:**

```c
set_cursor_pos(5, 3); // 6th column, 4th row
```

### 4. `void draw_char(char c)`

Renders a single character at current cursor position.

**Supported Characters:**

- ASCII 32-126 (printable)
- `\n` (newline)
- `\t` (4-space tab)

**Example:**

```c
draw_char('A');  // Draws 'A' at cursor
draw_char('\n'); // Moves to next line
```

### 5. `void print_text(const char *str)`

Renders null-terminated string.

**Example:**

```c
print_text("Hello\n\tWorld!");
// Output:
// Hello
//     World!
```

---

## Advanced Usage

### Scrolling (Not Implemented)

Current limitation - screen doesn't scroll when reaching bottom. To implement:

1. Track line count
2. Implement buffer shift when `cursor_y >= max_lines`

### Custom Fonts

To change font:

1. Convert font to PSF1 format
2. Update Makefile's `Font` variable
3. Recompile kernel

### Performance Tips

1. Batch text updates
2. Avoid frequent cursor changes
3. Pre-render static UI elements

---

## Technical Details

### Font Format

```c
struct psf1_header {
    uint8_t magic[2];   // 0x36 0x04
    uint8_t mode;       // 0 = 256 chars
    uint8_t char_size;  // Bytes per glyph
};
```

### Memory Layout

```
+-------------------+
| PSF Header        |
+-------------------+
| Glyph 0 (char 32) |
+-------------------+
| Glyph 1 (char 33) |
+-------------------+
| ...               |
```

---

## Known Limitations

1. No UTF-8/Unicode support
2. No text scrolling
3. Fixed 8x16 character size
4. Basic tab implementation (4 spaces)

---

## Example Usage

```c
// Kernel initialization
KInit(uint32_t magic, uint32_t *mb2_info) {
    struct multiboot_tag_framebuffer *fb_tag = mb2_get_fb_tag(mb2_info);
    graphics_init(fb_tag);
    init_font();
    
    set_text_color(0x00FF00, 0x000033);
    print_text("ProtOS Boot Complete\n");
    
    set_text_color(0xFFFFFF, 0x000000);
    print_text("Memory: 4GB\nStatus: OK");
    
    while(1) __asm__("hlt");
}
```
