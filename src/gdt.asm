; GDT Configuration
section .text
global load_gdt

gdt_start:
    dq 0x0000000000000000      ; Null descriptor

gdt_code:
    dw 0xFFFF                  ; Limit (low)
    dw 0x0000                  ; Base (low)
    db 0x00                    ; Base (middle)
    db 0x9A                    ; Access (exec/read)
    db 0xCF                    ; Flags | Limit (high)
    db 0x00                    ; Base (high)

gdt_data:
    dw 0xFFFF                  ; Limit (low)
    dw 0x0000                  ; Base (low)
    db 0x00                    ; Base (middle)
    db 0x92                    ; Access (read/write)
    db 0xCF                    ; Flags | Limit (high)
    db 0x00                    ; Base (high)
gdt_end:

gdt_ptr:
    dw gdt_end - gdt_start - 1 ; GDT limit
    dd gdt_start               ; GDT base

load_gdt:
    cli
    lgdt [gdt_ptr]
    jmp 0x08:flush_cs          ; Far jump to update CS
flush_cs:
    mov ax, 0x10               ; Update data segments
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    ret
