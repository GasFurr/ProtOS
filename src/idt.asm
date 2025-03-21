; Interrupt Descriptor Table
bits 32

%macro ISR_NOERRCODE 1
global isr%1
isr%1:
    cli
    push byte 0                ; Dummy error code
    push byte %1               ; Interrupt number
    jmp isr_common
%endmacro

%macro ISR_ERRCODE 1
global isr%1
isr%1:
    cli
    push byte %1               ; Interrupt number
    jmp isr_common
%endmacro

%macro IRQ 2
global irq%1
irq%1:
    cli
    push byte 0                ; Dummy error code
    push byte %2               ; Interrupt number
    jmp irq_common
%endmacro

; Exception Handlers
ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7
ISR_NOERRCODE 8
ISR_NOERRCODE 9
ISR_NOERRCODE 10
ISR_NOERRCODE 11
ISR_NOERRCODE 12
ISR_NOERRCODE 13
ISR_NOERRCODE 14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_NOERRCODE 17
ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_NOERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_NOERRCODE 30
ISR_NOERRCODE 31


; IRQ Handlers
IRQ 0, 32                      ; Timer
IRQ 1, 33                      ; Keyboard
; i dont givafak about adding all of tehm. 
extern isr_handler
isr_common:
    pusha
    mov ax, ds
    push eax
    mov ax, 0x10               ; Kernel data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    call isr_handler
    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    popa
    add esp, 8
    sti
    iret

extern irq_handler
irq_common:
    pusha
    mov ax, ds
    push eax
    mov ax, 0x10               ; Kernel data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    call irq_handler
    pop ebx
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx
    popa
    add esp, 8
    sti
    iret
