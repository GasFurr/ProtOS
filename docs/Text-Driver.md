---
# Text Driver Documentation (`text.c`)

## Overview
Provides framebuffer text rendering capabilities for your kernel using PSF2 fonts. Key features:
- Scaled text rendering (2x by default)
- Customizable colors and spacing
- Cursor positioning
- Basic text formatting (newlines, tabs)
- Automatic word wrapping

## Dependencies
- Graphic framebuffer driver (`graphic.c`)
- PSF2 font file compiled into kernel
- Multiboot2 framebuffer initialization

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
- Automatic word wrapping

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

---

## Usage Example

```c
// Kernel initialization
KInit(...) {
    graphics_init(fb_tag);
    init_font();
    
    // Configure output
    set_text_color(0x00FF00, 0x000033); // Green on dark blue
    font_scale = 2; // Double-size text
    line_spacing = 4;

    // Display output
    KOutput("ProtOS Kernel v0.1.8\n");
    KOutput("Memory: 4GB\tStatus: OK\n");
    KOutput("UEFI Firmware: ");
    KOutput(firmware_version);
}
```

---

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
- Pixel-perfect scaling with anti-aliasing

---

## Advanced Usage

### Debug Output

```c
// Combine with serial output
KOutput("Initializing devices...");
serial_puts("DEVICE INIT: Starting hardware detection\n");

// Error handling
if(!fb) {
    KOutput("FATAL: No framebuffer!");
    serial_puts("[ERROR] Framebuffer not found\n");
    while(1);
}
```

### Dynamic Configuration

```c
// Adjust for high-resolution displays
if(fb->framebuffer_width > 1920) {
    font_scale = 3;
    line_spacing = 6;
    char_spacing = 2;
}
```

---

## Performance Considerations

1. **Batch Operations**: Group `KOutput` calls
2. **Pre-Render Static Text**: During initialization
3. **Limit Redraws**: Update only changed regions
4. **Use Serial for Logging**: Less overhead than framebuffer

---

## Known Limitations

1. No UTF-8/Unicode support
2. Fixed tab size (4 characters)
3. No text scrolling implementation
4. Single font active at a time
