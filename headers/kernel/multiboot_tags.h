#ifndef MULTIBOOT_TAGS_H
#define MULTIBOOT_TAGS_H

#include <stdint.h>
/**
 * Multiboot2 Tag Header.
 * ------------------------------------------
 * Every tag in the Multiboot2 info structure starts with these fields.
 */
struct multiboot_tag {
  uint32_t type; // Tag type identifier (e.g., 6 = memory map, 8 = framebuffer)
  uint32_t size; // Total size of tag including header and payload (in bytes)
};

/**
 * Memory Map Entry (Type 6)
 * -------------------------
 * Describes a single contiguous region of physical memory.
 * Multiple entries form the complete memory map.
 */
struct multiboot_mmap_entry {
  uint64_t addr; // Starting physical address of memory region
  uint64_t len;  // Length of the region in bytes
  uint32_t type; // Memory type:
                 //   1 = Available RAM
                 //   2 = Reserved (unusable)
                 //   3 = ACPI reclaimable
                 //   4 = Reserved (NVS memory)
                 //   5 = Bad memory
  uint32_t zero; // Reserved padding - must be 0 (for 64-bit alignment)
};

/**
 * Memory Map Tag Structure (Type 6)
 * ---------------------------------
 * Contains the complete memory map with multiple entries.
 * Accessed via Multiboot2 info structure when tag type is 6.
 */
struct multiboot_tag_mmap {
  uint32_t type;          // Tag type = 6
  uint32_t size;          // Total size of this tag
  uint32_t entry_size;    // Size of each mmap entry (should be 24 bytes)
  uint32_t entry_version; // Format version (0 = current)
  struct multiboot_mmap_entry entries[]; // Variable-length array of entries
};

/**
 * Framebuffer Tag Structure (Type 8)
 * ----------------------------------
 * Provides display configuration details when using a linear framebuffer.
 */
struct multiboot_tag_framebuffer {
  uint32_t type;   // Tag type = 8
  uint32_t size;   // Total size of this tag
  uint64_t addr;   // Physical address of framebuffer
  uint32_t pitch;  // Bytes per scanline (may include padding)
  uint32_t width;  // Horizontal resolution in pixels
  uint32_t height; // Vertical resolution in pixels
  uint8_t bpp;     // Bits per pixel (common values: 24, 32)
};

/**
 * UEFI System Table Tag (Type 15)
 * -------------------------------
 * Provides access to UEFI firmware services.
 * Only present if booted via UEFI.
 */
struct multiboot_tag_uefi {
  uint32_t type;    // Tag type = 15
  uint32_t size;    // Total size of this tag
  uint64_t pointer; // Physical address of UEFI System Table
};

#endif
