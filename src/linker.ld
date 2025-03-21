/* Kernel Linker Script */
ENTRY(start)
OUTPUT_FORMAT(elf32-i386)

SECTIONS {
    . = 1M;                   /* Load kernel at 1MB */

    .text ALIGN(4K) : {
        *(.multiboot)         /* Multiboot header must be early */
        *(.text)              /* All code sections */
    }

    .rodata ALIGN(4K) : {
        *(.rodata*)           /* Read-only data */
    }

    .data ALIGN(4K) : {
        *(.data)              /* Initialized data */
    }

    .bss ALIGN(4K) : {
        *(COMMON)             /* Common symbols */
        *(.bss)              /* Uninitialized data */
    }

    /DISCARD/ : {             /* Remove unnecessary sections */
        *(.comment)
        *(.eh_frame)
    }
}
