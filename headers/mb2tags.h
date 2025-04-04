// Multiboot2 tags header file.
// It will contain all mb2 tag structures.
#ifndef MB2TAGS_H
#define MB2TAGS_H

#include <stddef.h>
#include <stdint.h>

// Multiboot2 tag type constants
#define MULTIBOOT_TAG_TYPE_FRAMEBUFFER 8

// Tag header (common to all tags)
struct multiboot_tag {
  uint32_t type;
  uint32_t size;
} __attribute__((packed));

// Framebuffer types
enum framebuffer_type {
  FB_TYPE_INDEXED = 0,
  FB_TYPE_DIRECTRGB = 1,
  FB_TYPE_EGATEXT = 2
};

// Framebuffer details structure
struct multiboot_tag_framebuffer {
  // Common tag header
  uint32_t type; // MULTIBOOT_TAG_TYPE_FRAMEBUFFER (8)
  uint32_t size; // Total size including color info

  // Framebuffer configuration
  uint64_t framebuffer_addr;   // Physical address of memory buffer
  uint32_t framebuffer_pitch;  // Bytes per horizontal line (stride)
  uint32_t framebuffer_width;  // Horizontal resolution in pixels
  uint32_t framebuffer_height; // Vertical resolution in pixels
  uint8_t framebuffer_bpp;     // Bits per pixel (color depth)
  uint8_t framebuffer_type;    // See framebuffer_type enum
  uint8_t reserved;            // Must be 0 (reserved for future use)

  // Type-specific color information
  union {
    struct {                    // Direct RGB color (FB_TYPE_DIRECTRGB)
      uint32_t red_position;    // Bit offset for red component
      uint32_t red_mask_size;   // Bit width for red component
      uint32_t green_position;  // Bit offset for green component
      uint32_t green_mask_size; // Bit width for green component
      uint32_t blue_position;   // Bit offset for blue component
      uint32_t blue_mask_size;  // Bit width for blue component
    } direct_rgb;

    struct {                       // Indexed color palette (FB_TYPE_INDEXED)
      uint16_t palette_num_colors; // Number of available colors
      uint8_t palette[];           // Flexible array for color data
    } indexed;
  };
} __attribute__((packed));

/**
 * Finds the first framebuffer tag in a Multiboot2 information structure
 *
 * @param info_ptr Pointer to start of Multiboot2 information structure
 * @return Pointer to framebuffer tag if found, NULL otherwise
 */
static inline struct multiboot_tag_framebuffer *mb2_get_fb_tag(void *info_ptr) {
  // Multiboot2 info header layout:
  // uint32_t total_size
  // uint32_t reserved (always 0)
  const struct multiboot_tag *current_tag =
      (struct multiboot_tag *)((char *)info_ptr + 8);
  const uint32_t total_size = *(uint32_t *)info_ptr;
  const char *end_address = (char *)info_ptr + total_size;

  while ((const char *)current_tag < end_address) {
    if (current_tag->type == MULTIBOOT_TAG_TYPE_FRAMEBUFFER) {
      return (struct multiboot_tag_framebuffer *)current_tag;
    }

    // Tags are 8-byte aligned: size + (8 - (size % 8)) % 8
    uint32_t tag_size_aligned = (current_tag->size + 7) & ~7;
    current_tag = (const struct multiboot_tag *)((const char *)current_tag +
                                                 tag_size_aligned);
  }

  return NULL;
}
#endif // MB2TAGS_H
