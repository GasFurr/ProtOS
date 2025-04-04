; \
;  >- Boot file. Where everything starts. 
; /
;
; First things first - multiboot2 header.

section .multiboot_header

header_start:
;   Magic number:
dd 0xE85250D6
;   Architecture:
dd 0 ; i386 or 32-bit protected mode.
;   Header length:
dd header_end - header_start
;   Checksum: -(magic + architecture + header_length)
dd 0x100000000 - (0xE85250D6 + 0 + (header_end - header_start))

;   Information request tag:
align 8
;   Type 1: Information request
dw 1
;   Flags
dw 0
;   Size = 24 (8 header + 3*4 + 4 padding)
dd 24
;   Request command line
dd 1
;   Request bootloader name
dd 2
;   Request memory map
dd 6
;   Padding:
dd 0

;   Framebuffer request tag:
align 8
;   Type 5: Framebuffer
dw 5
;   Flags
dw 0x1
;   Size 20 bytes (8 header + 3*4)
dd 20
;   Width: 0 - Just let GRUB choose.
dd 1920
;   Height:
dd 1080
;   Depth: 32 bits per pixel
dd 32

;   Kernel entry point tag:
align 8
;   Type 3: Entry adress
dw 3
;   Flags
dw 0
;   Size: 16 bytes
dd 16
;   Entry point
dd _start 
;   Padding
dd 0

;   End tag:
align 8
;   Type 0: End.
dw 0
;   Flags
dw 0
;   Size
dd 8
header_end:

section .text
extern KInit
global _start
_start:
;   Setting up stack.
  mov esp, stack_top
;   Pass Multiboot2 info pointer as argument (EBX holds the adress)
  push ebx
;   Pass Multiboot2 Magic Number
  push eax
;   Calling C kernel entry point.
  call KInit
;   If kernel_entry returns (should'nt happen) - hang.
  cli
.haltncatchfire:
  hlt
  jmp .haltncatchfire

section .bss
align 16
stack_bottom:
  resb 16384 ; 16KB stack.
stack_top:
