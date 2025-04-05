---

# 📁 GDT.md

## Global Descriptor Table Documentation

### Overview
Manages x86 protected mode memory segmentation through hardware-enforced privilege levels and memory access rules. Essential for modern kernel operation.

## Features
- 32-bit flat memory model
- Ring 0 (kernel) privilege enforcement
- Basic code/data segment isolation
- TSS preparation (future SMP support)

## API Reference

### `void gdt_init(void)`
Initializes core GDT structure with:  
- Null descriptor (0x00)  
- 4GB Code segment (0x08)  
- 4GB Data segment (0x10)  

**Must be called before any memory operations.**

## Technical Details

| Descriptor | Base  | Limit     | Type  | Privilege |
|------------|-------|-----------|-------|-----------|
| Null       | 0x0   | 0x0       | 0x00  | -         |
| Code       | 0x0   | 0xFFFFFFFF| 0x9A  | Ring 0    |
| Data       | 0x0   | 0xFFFFFFFF| 0x92  | Ring 0    |

## Limitations
1. No TSS implementation yet
2. Fixed 4GB segments
3. No user-mode segments

---

# 📁 IDT.md

## Interrupt Descriptor Table Documentation

### Overview
Handles CPU exceptions and future hardware interrupts through 256 configurable gates. Critical for system stability and debugging.

## Features
- Full exception handling (0-31)
- Custom C handler routing
- PIC remapping to 0x20-0x2F
- Maskable interrupts

## API Reference

### `void idt_init(void)`
Configures IDT with:
- 32 CPU exception gates
- 224 free interrupt slots
- Kernel-level privilege gates
- Default error code handling

### `void idt_set_entry(uint8_t num, void (*handler)())`
```c
// Set custom handler for interrupt 0x80
idt_set_entry(0x80, (uint32_t)syscall_handler);
```

## Technical Details

| Interrupt Range | Purpose               |
|-----------------|-----------------------|
| 0x00-0x1F       | CPU Exceptions        |
| 0x20-0x2F       | Remapped PIC IRQs     |
| 0x30-0xFF       | Available for drivers |

## Limitations
1. No IRQ handlers implemented
2. Missing spurious IRQ handling
3. Shared PIC masks

---


