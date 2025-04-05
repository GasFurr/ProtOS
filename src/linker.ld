/* Linker file */
ENTRY(_start)  /* Must match assembly entry point */

PHDRS {
    headers PT_LOAD FLAGS(5);  /* R-X for multiboot header */
    text PT_LOAD FLAGS(5);     /* R-X */
    rodata PT_LOAD FLAGS(4);   /* R-- */
    data PT_LOAD FLAGS(6);     /* RW- */
}

SECTIONS {
    /* Kernel base address */
    . = 1M;

    /* Multiboot header MUST be first */
    .multiboot_header : ALIGN(4K) {
        KEEP(*(.multiboot_header))
    } :headers

    /* Code section */
    .text : ALIGN(4K) {
        *(.text .text.*)
    } :text

    /* Read-only data */
    .rodata : ALIGN(4K) {
        *(.rodata .rodata.*)
        *(.font)  /* PSF2 font data */
    } :rodata

    /* Initialized data */
    .data : ALIGN(4K) {
        *(.data .data.*)
    } :data

    /* Uninitialized data */
    .bss : ALIGN(4K) {
        *(COMMON)
        *(.bss .bss.*)
    } :data

    /* Debug sections */
    /DISCARD/ : {
        *(.eh_frame)
        *(.note.*)
        *(.comment)
    }
}
