# Serial Output Driver Documentation

## Overview

Provides low-level serial port communication capabilities for kernel debugging and text output. Implements basic COM1 port handling at 9600 baud rate. Designed for integration with QEMU's `-serial stdio` option.

## Features

- 9600 baud rate communication
- Blocking character output
- Automatic CR/LF conversion
- Basic string output
- Early boot debugging support

## API Reference

### 1. `void serial_init(void)`

Initializes COM1 port with standard PC settings.  
**Must be called first** before any output operations.

### 2. `void serial_putc(char c)`

Outputs single character to serial port.  
Handles:

- LF -> CR/LF conversion
- Blocking until port is ready
- ASCII character output

### 3. `void serial_puts(const char *str)`

Outputs null-terminated string.  
Automatically handles:

- Newline conversions
- String length detection
- Sequential character transmission

### 4. `void serial_puthex(uint32_t val)`

```c
// Outputs: "0x00001A3F"
serial_puthex(0x1A3F); 
```

---

## Usage Example

```c
// Kernel initialization
KInit(...) {
    serial_init();
    serial_puts("\n\n=== ProtOS Boot Sequence ===\n");
    
    // Mixed debug output
    serial_puts("Initializing memory manager... ");
    init_memory();
    serial_puts("[OK]\n");
    
    // Error handling
    if(!fb) {
        serial_puts("[ERROR] No framebuffer detected!\n");
        while(1);
    }
}
```

```c

void debug_memory(void *ptr) {
    serial_puts("Accessing memory at: ");
    serial_puthex((uint32_t)ptr);
    serial_puts("\n");
    
    uint32_t value = *((uint32_t*)ptr);
    serial_puts("Read value: ");
    serial_puthex(value);
}
```

## Technical Details

### Port Configuration

| Setting              | Value          |
|----------------------|----------------|
| Base port            | 0x3F8 (COM1)  |
| Baud rate            | 9600           |
| Data bits            | 8              |
| Stop bits            | 1              |
| Parity               | None           |
| FIFO buffer          | 14-byte        |

### Character Handling

| Input | Output    | Description              |
|-------|-----------|--------------------------|
| \n    | \r\n      | Automatic newline fix    |
| \t    | 4 spaces  | Tab expansion            |
| Other | Direct    | ASCII character pass-through |

## Limitations

1. No input handling (read-only implementation)
2. Fixed to COM1 port
3. No baud rate configuration
4. Blocking implementation (no interrupts)

---

> **Note**: This serial implementation is primarily for debugging purposes. For user-facing text output, use the framebuffer-based `KOutput()` function from the text driver.
