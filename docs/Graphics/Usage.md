# Getting Started

## Basic Setup

```c
struct multiboot_tag_framebuffer *fb = mb2_get_fb_tag(mb2_info);
if(fb && fb->framebuffer_type == FB_TYPE_DIRECTRGB) {
    graphics_init(fb);
}
```

## Drawing Example

```c
// Draw red diagonal line
draw_line(0, 0, 
          fb->framebuffer_width-1, fb->framebuffer_height-1,
          0xFF0000);

// Draw blue filled rectangle
draw_rect(100, 100, 200, 150, 0x0000FF, 1);

// Clear to green
clear_screen(0x00FF00);
```

## Important Notes

Coordinates start at top-left corner
Only works with 32-bit direct RGB framebuffers
No bounds checking on purpose - draw within screen limits
Not thread-safe - assume single caller
