#include "serial.h"
#include <io.h>
#include <kb.h>
#include <pic.h>
#include <stdint.h>
#include <testshell.h>
#include <text.h>

static uint8_t shift_pressed = 0;
static uint8_t caps_lock = 0;
char input_buffer[256];
uint16_t buffer_index = 0;

static char scancode_to_ascii(uint8_t scancode) {
  static const char keyboard_map[] = {
      0,   0,   '1',  '2', '3',  '4', '5', '6',  '7', '8', '9', '0',
      '-', '=', '\b', 0,   'q',  'w', 'e', 'r',  't', 'y', 'u', 'i',
      'o', 'p', '[',  ']', '\n', 0,   'a', 's',  'd', 'f', 'g', 'h',
      'j', 'k', 'l',  ';', '\'', '`', 0,   '\\', 'z', 'x', 'c', 'v',
      'b', 'n', 'm',  ',', '.',  '/', 0,   0,    0,   ' '};

  static const char shifted_map[] = {
      0,   0,   '!',  '@', '#',  '$', '%', '^', '&', '*', '(', ')',
      '_', '+', '\b', 0,   'Q',  'W', 'E', 'R', 'T', 'Y', 'U', 'I',
      'O', 'P', '{',  '}', '\n', 0,   'A', 'S', 'D', 'F', 'G', 'H',
      'J', 'K', 'L',  ':', '"',  '~', 0,   '|', 'Z', 'X', 'C', 'V',
      'B', 'N', 'M',  '<', '>',  '?', 0,   0,   0,   ' '};

  if (scancode >= 0x80)
    return 0; // Ignore key releases

  // Use shifted map if Shift is pressed (XOR with Caps Lock for toggle)
  const char *current_map =
      (shift_pressed ^ caps_lock) ? shifted_map : keyboard_map;

  if (scancode >= sizeof(keyboard_map))
    return 0;
  return current_map[scancode];
}

void keyboard_handler(uint8_t scancode) {
  serial_puts("Keyboard handling active!\n");
  // Handle modifier keys
  switch (scancode) {
  case 0x2A: // Left Shift pressed
  case 0x36: // Right Shift pressed
    shift_pressed = 1;
    return;

  case 0xAA: // Left Shift released
  case 0xB6: // Right Shift released
    shift_pressed = 0;
    return;

  case 0x3A:        // Caps Lock pressed
    caps_lock ^= 1; // Toggle state
    return;
  }

  if (scancode == 0x1C) {
    execute_command();           // Enter
  } else if (scancode == 0x0E) { // Backspace
    if (buffer_index > 0) {
      // Remove from buffer first
      buffer_index--;
      input_buffer[buffer_index] = '\0';
      // Clear the character from screen
      erase_char();
    }
  } else {
    char c = scancode_to_ascii(scancode);
    if (c && buffer_index < sizeof(input_buffer) - 1) {
      input_buffer[buffer_index++] = c;
      draw_char(c);
      serial_putc(c);
    }
  }
}

void keyboard_interrupt_handler(void) {
  uint8_t scancode = inb(0x60);
  keyboard_handler(scancode);
  // EOI is now handled in irq_handler
}

void keyboard_init(void) { outb(PIC1_DATA, inb(PIC1_DATA) & ~0x02); }
