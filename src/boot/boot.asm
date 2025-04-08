; \
;  >- Boot file. Where everything starts. 
; /
;
; First things first - multiboot2 header.

section .multiboot_header
header_start:
    dd 0xE85250D6                ; Magic number
    dd 0                         ; Architecture (i386)
    dd header_end - header_start ; Header length
    ; Checksum
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
;   Width: 1920
dd 1920
;   Height: 1080
dd 1080
;   Unfortunately, when i set it to 0 so
;   give choice to GRUB it just breaks. 
;   I dont know why and how to fix it.

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
global _start
extern gdt_init
extern isr_handler     ; C interrupt handler
extern KInit           ; Kernel entry

_start:
    cli
    
    ; Set up stack
    mov esp, stack_top
    and esp, -16
    
    ; Initialize GDT BEFORE loading it
    call gdt_init    ; This must come first!
    call gdt_flush   ; Now safe to load GDT
    
    call idt_load
    ; Pass args and enter kernel
    push ebx
    push eax
    call KInit
    
    cli
.hang: hlt
    jmp .hang

global gdt_flush
extern gdtp
gdt_flush:
    lgdt [gdtp]
    jmp 0x08:.reload_cs
.reload_cs:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    ret

; ISR macros for exceptions
%macro ISR_NOERRCODE 1
global isr%1
isr%1:
    push byte 0        ; Dummy error code
    push byte %1       ; Interrupt number
    jmp isr_common
%endmacro

%macro ISR_ERRCODE 1
global isr%1
isr%1:
    push byte %1       ; Interrupt number (error code already pushed)
    jmp isr_common
%endmacro

; Generate ISR stubs for CPU exceptions
ISR_NOERRCODE 0       ; Divide Error
ISR_NOERRCODE 1       ; Debug
ISR_NOERRCODE 2       ; NMI
ISR_NOERRCODE 3       ; Breakpoint
ISR_NOERRCODE 4       ; Overflow
ISR_NOERRCODE 5       ; Bound Range
ISR_NOERRCODE 6       ; Invalid Opcode
ISR_NOERRCODE 7       ; Device Not Available
ISR_ERRCODE   8       ; Double Fault
ISR_NOERRCODE 9       ; Coprocessor Segment
ISR_ERRCODE   10      ; Invalid TSS
ISR_ERRCODE   11      ; Segment Not Present
ISR_ERRCODE   12      ; Stack Fault
ISR_ERRCODE   13      ; General Protection Fault
ISR_ERRCODE   14      ; Page Fault
ISR_NOERRCODE 15      ; Reserved
ISR_NOERRCODE 16      ; x87 FPU error // math fault.
ISR_ERRCODE   17      ; Alignment check
ISR_NOERRCODE 18      ; Machine check
ISR_NOERRCODE 19      ; SIMD floating point exception
ISR_NOERRCODE 20      ; Virtualization exception
ISR_ERRCODE   21      ; Control protection exception
ISR_NOERRCODE 22      ; Reserved for future as CPU exception vectors
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_NOERRCODE 30
ISR_NOERRCODE 31

; Common ISR handler (saves state and calls C function)
isr_common:
    pusha              ; Push edi, esi, ebp, esp, ebx, edx, ecx, eax
    mov ax, ds
    push eax           ; Save data segment

    mov ax, 0x10       ; Load kernel data descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp           ; Pass registers pointer to C
    call isr_handler
    add esp, 4

    pop eax            ; Restore original data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popa               ; Restore registers
    add esp, 8         ; Clean error code and int number
    iret               ; Return from interrupt

extern irq_handler

%macro IRQ 2
global irq%1
irq%1:
    push byte 0     ; Dummy error code
    push byte %2    ; Interrupt number
    jmp irq_common
%endmacro

; Create IRQ stubs 0-15
IRQ 0, 32  ; Timer
IRQ 1, 33  ; Keyboard
IRQ 2, 34
; ... continue for other IRQs

irq_common:
    pusha              ; Save all registers
    mov ax, ds
    push eax           ; Save data segment

    mov ax, 0x10       ; Load kernel data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp           ; Pass registers pointer to C
    call irq_handler
    add esp, 4

    pop ebx            ; Restore original data segment
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx

    popa               ; Restore registers
    add esp, 8         ; Clean error code and int number
    iret

; Remap PIC to avoid conflicts with CPU exceptions
remap_pic:
    mov al, 0x11
    out 0x20, al
    out 0xA0, al

    mov al, 0x20
    out 0x21, al
    mov al, 0x28
    out 0xA1, al

    mov al, 0x04
    out 0x21, al
    mov al, 0x02
    out 0xA1, al

    mov al, 0x01
    out 0x21, al
    out 0xA1, al

    mov al, 0xFF
    out 0xA1, al
    out 0x21, al
    ret

; Load IDT
global idt_load
idt_load:
    lidt [idt_descriptor]
    ret

section .data
align 4
global idt_descriptor
idt_descriptor:
    dw 0         ; Size (set in C)
    dd 0         ; Address (set in C)

section .bss
align 16
stack_bottom:
    resb 16384
stack_top:
