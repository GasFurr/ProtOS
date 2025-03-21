; Bootloader Section
section .multiboot
header_start:
    dd 0x1BADB002              ; Magic number
    dd 0x00                    ; Flags
    dd -(0x1BADB002 + 0x00)    ; Checksum

section .text
global start
extern kernel_main
extern load_gdt

start:
    mov esp, 0x90000           ; Setup stack
    call load_gdt               ; Initialize GDT
    call kernel_main           ; Jump to kernel
    cli                        ; Stop interrupts
.hang:
    hlt
    jmp .hang
