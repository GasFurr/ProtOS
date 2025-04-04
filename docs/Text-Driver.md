---
# Text Driver Documentation (`text.c`)

## Overview
Provides framebuffer text rendering capabilities using PSF2 fonts. Key features:
- Scaled text rendering (2x by default)
- Customizable colors and spacing
- Cursor positioning
- Basic text formatting (newlines, tabs)

---

## API Reference

### 1. `void init_font(void)`

Initializes the font system using embedded PSF2 font data.  
**Must be called after** framebuffer initialization.

### 2. `void KOutput(const char *str)`

**Primary text output function** - Renders a null-terminated string.  
Supports:

- `\n` Newlines
- `\t` Tabs (4 character width)

### 3. `void set_text_color(uint32_t fg, uint32_t bg)`

Sets foreground and background colors.  
Parameters:

- `fg`: Foreground color (24-bit RGB)
- `bg`: Background color (24-bit RGB)

### 4. `void set_cursor_pos(uint32_t x, uint32_t y)`

Sets text cursor position in **character grid coordinates**  
Example: `set_cursor_pos(2, 3)` = Column 3, Row 4

### 5. Configuration Variables

```c
extern uint32_t font_scale;    // Rendering scale (1-4)
extern uint32_t line_spacing;  // Pixels between lines
extern uint32_t char_spacing;  // Pixels between characters
```

## Technical Details

### Font Requirements

- **Format**: PSF2 (Magic: `0x72 0xb5 0x4a 0x86`)
- **Recommended Size**: 8x16 or 16x32 pixels
- **Placement**: Compiled into kernel using objcopy

### Memory Layout

```
+-------------------+
| PSF2 Header       | 32 bytes
+-------------------+
| Glyph Data        | (num_glyphs * bytes_per_glyph)
+-------------------+
```

### Character Rendering

- Each character drawn as scaled bitmap
- Background pixels explicitly filled
- Pixel-perfect scaling

---

## Advanced Usage

```

## Known Limitations
1. No UTF-8/Unicode support
2. Fixed tab size (4 characters)
3. No text scrolling implementation
4. Single font active at a time

---
