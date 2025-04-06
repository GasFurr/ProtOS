#include "kb.h"
#include "io.h"

#define BUFFER_SIZE 128

// Scancode set 1 translation table (US QWERTY)
static const char keymap[] = {
    0,   0x1B, '1',  '2', '3',  '4', '5', '6', '7', '8', '9', '0', '-',
    '=', '\b', '\t', 'q', 'w',  'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',
    '[', ']',  '\n', 0,   'a',  's', 'd', 'f', 'g', 'h', 'j', 'k', 'l',
    ';', '\'', '`',  0,   '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',',
    '.', '/',  0,    '*', 0,    ' ', 0,   0,   0,   0,   0,   0,   0,
    0,   0,    0,    0,   0,    0,   0,   0,   0,   '-', 0,   0,   0,
    '+', 0,    0,    0,   0,    0,   0,   0};

static volatile char buffer[BUFFER_SIZE];
static volatile uint32_t head = 0;
static volatile uint32_t tail = 0;
static int shift = 0;

static void buffer_put(char c) {
  uint32_t next = (head + 1) % BUFFER_SIZE;
  if (next != tail) {
    buffer[head] = c;
    head = next;
  }
}

void keyboard_handler() {
  uint8_t scancode = inb(0x60);

  // Handle key release
  if (scancode & 0x80) {
    uint8_t code = scancode & 0x7F;
    if (code == 0x2A || code == 0x36)
      shift = 0;
    return;
  }

  // Handle modifier keys
  if (scancode == 0x2A || scancode == 0x36) {
    shift = 1;
    return;
  }

  // Translate scancode
  if (scancode < sizeof(keymap)) {
    char c = keymap[scancode];

    if (shift) {
      if (c >= 'a' && c <= 'z')
        c -= 32;
      else
        switch (c) { // Handle shifted symbols
        case '1':
          c = '!';
          break;
        case '2':
          c = '@';
          break;
        case '3':
          c = '#';
          break;
        case '4':
          c = '$';
          break;
        case '5':
          c = '%';
          break;
        case '6':
          c = '^';
          break;
        case '7':
          c = '&';
          break;
        case '8':
          c = '*';
          break;
        case '9':
          c = '(';
          break;
        case '0':
          c = ')';
          break;
        case '-':
          c = '_';
          break;
        case '=':
          c = '+';
          break;
        case '[':
          c = '{';
          break;
        case ']':
          c = '}';
          break;
        case ';':
          c = ':';
          break;
        case '\'':
          c = '"';
          break;
        case '`':
          c = '~';
          break;
        case '\\':
          c = '|';
          break;
        case ',':
          c = '<';
          break;
        case '.':
          c = '>';
          break;
        case '/':
          c = '?';
          break;
        }
    }

    buffer_put(c);
  }

  outb(0x20, 0x20); // EOI to PIC
}

void keyboard_init() {
  // Enable keyboard interrupts
  outb(0x21, inb(0x21) & ~0x02);

  // Enable keyboard device
  while (inb(0x64) & 0x02)
    ;
  outb(0x64, 0xAE);
}

char keyboard_read() {
  if (head == tail)
    return 0;

  char c = buffer[tail];
  tail = (tail + 1) % BUFFER_SIZE;
  return c;
}
