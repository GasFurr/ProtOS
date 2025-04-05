# 📁 Exceptions.md

## Exception Handling Documentation

### Overview
Provides detailed diagnostics for 32 x86 CPU exceptions through serial output and automatic halting.

## Features
- Human-readable error names
- Full register state dump
- Faulting instruction tracking
- Page fault address reporting

## Data Structure
```c
struct registers {
    uint32_t ds, edi, esi, ebp, esp;
    uint32_t ebx, edx, ecx, eax;
    uint32_t int_num, err_code;
    uint32_t eip, cs, eflags;
};
```

## Error Reporting Example
```
! EXCEPTION: Page Fault
INT: 0x0E ERR: 0x00000000 EIP: 0x00102A3F
CR2: 0xDEADBEEF (Faulting address)
EAX:0x00000001 EBX:0x00000000 ECX:0x00000000 
ESI:0x00108000 EDI:0x00000000 EBP:0x00107FF0
```

## Handling Flow
1. Save processor state
2. Identify exception type
3. Dump registers to serial
4. Halt execution indefinitely


