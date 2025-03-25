; Multiboot2 header for GRUB
section .multiboot_header
header_start:
    dd 0xE85250D6                ; Magic number (Multiboot2)
    dd 0                         ; Architecture (0 = x86)
    dd header_end - header_start ; Header length
    dd -(0xE85250D6 + 0 + (header_end - header_start)) ; Checksum

    ; Entry address tag
    align 8
    dw 3                         ; Type: entry address
    dw 0                         ; Flags
    dd 12                        ; Size
    ; End tag
    align 8
    dw 0                         ; Type: end
    dw 0                         ; Flags
    dd 8                         ; Size
header_end:


section .text
global start
extern kernel_entry
start:
  mov esp, stack_top
  push ebx
  call kernel_entry
  hlt
  

section .bss
stack_bottom:
  resb 16384
stack_top:

