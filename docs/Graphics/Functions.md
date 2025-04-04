# Framebuffer Graphics Library

Simple drawing functions for a linear framebuffer in 32-bit RGB mode.

## Core Functions

### `graphics_init(fb_tag)`

- Initialize the graphics system
- **Parameters**:
  - `fb_tag`: Valid Multiboot2 framebuffer tag
- **Must be called first** before any drawing operations

### `draw_pixel(x, y, color)`

- Draw a single pixel
- **Parameters**:
  - `x`, `y`: Screen coordinates (0,0 = top-left)
  - `color`: 24-bit RGB color (0xRRGGBB)

### `clear_screen(color)`

- Fill entire screen with a color
- **Parameters**:
  - `color`: 24-bit RGB fill color

### `draw_rect(x, y, w, h, color, filled)`

- Draw a rectangle
- **Parameters**:
  - `x`, `y`: Top-left position
  - `w`, `h`: Width and height
  - `color`: 24-bit RGB color
  - `filled`: 0=outline, 1=filled

### `draw_line(x0, y0, x1, y1, color, thickness)`

- Draw a straight line using Bresenham's algorithm
- **Parameters**:
  - Start (`x0`,`y0`) and end (`x1`,`y1`) points
  - `color`: 24-bit RGB color
  - thickness: unsigned integer. Thickness = parameter \ 2. Example: if thickness = 10 it will be +5 pixel on both sides of the line.

## Color Format

- Colors are specified as 24-bit RGB values
- Example: `0xFF0000` = red, `0x00FF00` = green, `0x0000FF` = blue
- Actual displayed color depends on framebuffer configuration
