# Multiboot2 Framebuffer Support

## Key Structures

### `struct multiboot_tag_framebuffer`

Contains framebuffer configuration:

- Physical address
- Resolution (width/height)
- Color depth (bpp)
- Pitch (bytes per scanline)
- Color mask information

## Helper Function

### `mb2_get_fb_tag(info)`

- Find framebuffer tag in Multiboot2 info
- **Parameters**:
  - `info`: Pointer to Multiboot2 info structure
- **Returns**: Framebuffer tag pointer or NULL

## Usage Flow

1. Get Multiboot2 info from bootloader
2. Use `mb2_get_fb_tag()` to find framebuffer
3. Verify framebuffer is in direct RGB mode
4. Pass to `graphics_init()`
