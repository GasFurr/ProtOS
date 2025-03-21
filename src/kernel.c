/* kernel.c - Main kernel implementation */
#include "kernel.h"
#include "commands.h"
#include <stdint.h>

/* ==================== Constants & Macros ==================== */
#define VGA_ADDRESS        0xB8000
#define WHITE_ON_BLACK     0x0F
#define SCREEN_WIDTH       80
#define SCREEN_HEIGHT      25
#define PIC1_CMD          0x20
#define PIC1_DATA         0x21
#define PIC2_CMD          0xA0
#define PIC2_DATA         0xA1

/* ==================== Global Variables ==================== */
volatile char *vga = (volatile char*)VGA_ADDRESS;
static uint8_t cursor_x = 0;
static uint8_t cursor_y = 0;
char input_buffer[256];
uint16_t buffer_index = 0;

/* ==================== Forward Declarations ==================== */
static void update_cursor(void);
static char scancode_to_ascii(uint8_t scancode);
extern uint32_t shutdown_timeout;

/* ==================== VGA Text Mode Functions ==================== */
void print_char(char c) {
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else {
        vga[(cursor_y * SCREEN_WIDTH + cursor_x) * 2] = c;
        vga[(cursor_y * SCREEN_WIDTH + cursor_x) * 2 + 1] = WHITE_ON_BLACK;
        cursor_x++;
    }

    /* Handle screen boundaries */
    if (cursor_x >= SCREEN_WIDTH) {
        cursor_x = 0;
        cursor_y++;
    }
    if (cursor_y >= SCREEN_HEIGHT) {
        cursor_y = SCREEN_HEIGHT - 1;
    }
    
    update_cursor();
}

void print(const char *str) {
    while (*str) print_char(*str++);
}

void clear_screen(void) {
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            const int index = (y * SCREEN_WIDTH + x) * 2;
            vga[index] = ' ';
            vga[index + 1] = WHITE_ON_BLACK;
        }
    }
    cursor_x = cursor_y = 0;
    update_cursor();
}

/* ==================== Memory Utilities ==================== */
void mem_set(void *ptr, uint8_t value, uint32_t size) {
    uint8_t *p = (uint8_t*)ptr;
    for (uint32_t i = 0; i < size; i++)
        p[i] = value;
}

/* ==================== String Utilities ==================== */
int strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) { s1++; s2++; }
    return *(uint8_t*)s1 - *(uint8_t*)s2;
}

void prompt(){
    print("ProtOS > ");
};

/* ==================== Keyboard Handling ==================== */
static uint8_t shift_pressed = 0;
static uint8_t caps_lock = 0;

void keyboard_handler(uint8_t scancode) {
    // Handle modifier keys
    switch(scancode) {
        case 0x2A:  // Left Shift pressed
        case 0x36:  // Right Shift pressed
            shift_pressed = 1;
            return;
            
        case 0xAA:  // Left Shift released
        case 0xB6:  // Right Shift released
            shift_pressed = 0;
            return;
            
        case 0x3A:  // Caps Lock pressed
            caps_lock ^= 1;  // Toggle state
            return;
    }

    if (scancode == 0x1C) { // Enter
        execute_command();
    } else if (scancode == 0x0E) { // Backspace
        if (buffer_index > 0) {
            // Remove from buffer first
            buffer_index--;
            input_buffer[buffer_index] = '\0';

            // Handle cursor movement
            if (cursor_x == 0) {
                if (cursor_y > 0) {
                    cursor_y--;
                    cursor_x = SCREEN_WIDTH - 1;
                }
            } else {
                cursor_x--;
            }

            // Clear the character from screen
            vga[(cursor_y * SCREEN_WIDTH + cursor_x) * 2] = ' ';
            update_cursor();
        }
    } else {
        char c = scancode_to_ascii(scancode);
        if (c && buffer_index < sizeof(input_buffer) - 1) {
            input_buffer[buffer_index++] = c;
            print_char(c);
        }
    }
}

/* ==================== Interrupt Handling ==================== */
__attribute__((naked)) void gpf_handler(void) {
    __asm__("pusha");
    __asm__("push %ds; push %es; push %fs; push %gs");
    __asm__("mov $0x10, %ax");
    __asm__("mov %ax, %ds; mov %ax, %es; mov %ax, %fs; mov %ax, %gs");
    __asm__("pushl 0x30(%esp); pushl $0x0D");  /* Error code & int number */
    __asm__("call gpf_handler_impl; add $8, %esp");
    __asm__("pop %gs; pop %fs; pop %es; pop %ds; popa; iret");
}

void timer_handler_wrapper(void) {
    __asm__("pusha");
    timer_handler();  
    outb(0x20, 0x20);
    __asm__("popa; iretl");
}

void gpf_handler_impl(uint32_t int_no, uint32_t error_code) {
    (void)int_no; (void)error_code;  /* TODO: Implement proper error handling */
    print("! GPF Occured !");
    while(1);
}

/* ==================== IDT Management ==================== */
struct idt_entry {
    uint16_t base_low;
    uint16_t selector;
    uint8_t zero;
    uint8_t flags;
    uint16_t base_high;
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

static struct idt_entry idt[256];
static struct idt_ptr idt_desc;

void idt_set_entry(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].base_low = base & 0xFFFF;
    idt[num].base_high = (base >> 16) & 0xFFFF;
    idt[num].selector = sel;
    idt[num].zero = 0;
    idt[num].flags = flags;
}

void idt_init(void) {
    idt_desc.limit = sizeof(idt) - 1;
    idt_desc.base = (uint32_t)&idt;

    /* Configure IDT entries */
    for (int i = 0; i < 256; i++)
        idt_set_entry(i, (uint32_t)default_handler, 0x08, 0x8E);

    idt_set_entry(13, (uint32_t)gpf_handler, 0x08, 0x8E);  /* GPF */
    idt_set_entry(32, (uint32_t)timer_handler_wrapper, 0x08, 0x8E); /* Timer */
    idt_set_entry(33, (uint32_t)keyboard_interrupt_wrapper, 0x08, 0x8E); /* Keyboard */

    __asm__("lidt %0" : : "m"(idt_desc));
}

/* ==================== PIC Configuration ==================== */
void pic_remap(void) {
    outb(PIC1_CMD, 0x11); outb(PIC2_CMD, 0x11);  /* ICW1: Initialize */
    outb(PIC1_DATA, 0x20); outb(PIC2_DATA, 0x28); /* ICW2: Vector offsets */
    outb(PIC1_DATA, 0x04); outb(PIC2_DATA, 0x02); /* ICW3: Master/Slave */
    outb(PIC1_DATA, 0x01); outb(PIC2_DATA, 0x01); /* ICW4: 8086 mode */
    outb(PIC1_DATA, 0xFD);                        /* Unmask IRQ0 & IRQ1 */
}

/* ==================== Helper Functions ==================== */
static void update_cursor(void) {
    uint16_t pos = cursor_y * SCREEN_WIDTH + cursor_x;
    outb(0x3D4, 0x0E); outb(0x3D5, (pos >> 8) & 0xFF);
    outb(0x3D4, 0x0F); outb(0x3D5, pos & 0xFF);
}

void print_dec(uint32_t num) {
    char buf[16];
    char *ptr = &buf[15];
    *ptr = '\0';
    
    do {
        *--ptr = '0' + (num % 10);
        num /= 10;
    } while(num > 0);
    
    print(ptr);
}

static char scancode_to_ascii(uint8_t scancode) {
    static const char keyboard_map[] = {
        0, 0, '1','2','3','4','5','6','7','8','9','0','-','=','\b',
        0, 'q','w','e','r','t','y','u','i','o','p','[',']','\n',
        0, 'a','s','d','f','g','h','j','k','l',';','\'','`',
        0, '\\','z','x','c','v','b','n','m',',','.','/',0,0,0,' '
    };

    static const char shifted_map[] = {
        0, 0, '!','@','#','$','%','^','&','*','(',')','_','+','\b',
        0, 'Q','W','E','R','T','Y','U','I','O','P','{','}','\n',
        0, 'A','S','D','F','G','H','J','K','L',':','"','~',
        0, '|','Z','X','C','V','B','N','M','<','>','?',0,0,0,' '
    };

    if (scancode >= 0x80) return 0;  // Ignore key releases
    
    // Use shifted map if Shift is pressed (XOR with Caps Lock for toggle)
    const char* current_map = (shift_pressed ^ caps_lock) ? shifted_map : keyboard_map;
    
    if (scancode >= sizeof(keyboard_map)) return 0;
    return current_map[scancode];
}


/* ==================== More Handlers ==================== */

void default_handler(void) {
    print("! Default Handler Triggered !");
    while(1);
}

void isr_handler(uint32_t int_no) {
    char msg[] = "ISR 0x00";
    msg[5] = (int_no >> 4) < 10 ? (int_no >> 4) + '0' : (int_no >> 4) - 10 + 'A';
    msg[6] = (int_no & 0xF) < 10 ? (int_no & 0xF) + '0' : (int_no & 0xF) - 10 + 'A';
    print(msg);
    while(1);
}

void irq_handler(uint32_t int_no) {
    char msg[] = "IRQ 0x00";
    msg[5] = (int_no >> 4) < 10 ? (int_no >> 4) + '0' : (int_no >> 4) - 10 + 'A';
    msg[6] = (int_no & 0xF) < 10 ? (int_no & 0xF) + '0' : (int_no & 0xF) - 10 + 'A';
    print(msg);
}

/* ==================== Main Kernel Entry ==================== */
void kernel_main(void) {
    print("Booting ProtOS...\n");
    load_gdt();
    idt_init();
    pic_remap();
    timer_init(100);  /* 100Hz timer */
    
    __asm__("sti");   /* Enable interrupts */
    clear_screen();
    prompt();
    
    while(1) {
        __asm__("hlt");
        if(shutdown_timeout > 0 && timer_ticks >= shutdown_timeout) {
            print("\nShutting down now!\n");
            outw(0x604, 0x2000);
        }
    }
}
