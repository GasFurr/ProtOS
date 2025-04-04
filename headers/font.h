// font.h
#ifndef FONT_H
#define FONT_H

#include <stdint.h>

// PSF2 Header (32 bytes)
struct psf2_header {
  uint8_t magic[4];
  uint32_t version;
  uint32_t headersize;
  uint32_t flags;
  uint32_t num_glyphs;
  uint32_t bytes_per_glyph;
  uint32_t height;
  uint32_t width;
  uint32_t reserved[4]; // PSF2 has 32 byte header
};

// Font symbols
extern const struct psf2_header _binary_font_psf_start;
extern const uint8_t _binary_font_psf_end[];

#endif
