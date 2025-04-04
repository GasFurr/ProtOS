/* Linker file */
ENTRY(KInit)

SECTIONS {
    . = 1M;

    .multiboot_header : {
        *(.multiboot_header)
    }

    .text : {
        *(.text .text.*)
    }

    .rodata : {
        *(.rodata .rodata.*)
    }

    .data : {
        *(.data .data.*)
    }

    .font : {
        KEEP(*(.font))
    }

    .bss : {
        *(COMMON)
        *(.bss .bss.*)
    }

    /DISCARD/ : {
        *(.eh_frame)
        *(.note.*)
        *(.comment)
    }
}
