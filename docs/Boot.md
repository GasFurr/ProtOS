### **Kernel Boot Process Documentation**  

A minimal, step-by-step explanation of how ProtOS kernel boots from power-on to graphical/text output:

---

### **1. Power-On & Firmware Initialization**

- **BIOS/UEFI** activates hardware, performs POST checks  
- **Boot Device Selection**: BIOS looks for bootable devices in order (HDD, USB, CD, etc.)  
- **Bootloader Loading**: BIOS transfers control to the bootloader (GRUB)  

---

### **2. GRUB Bootloader Phase**

- **Stage 1**: GRUB MBR loads core image  
- **Stage 2**: GRUB reads configuration (`grub.cfg`)  
- **Multiboot2 Header Detection**: GRUB scans kernel binary for header in first 32KB  

```nasm
; Required Multiboot2 Header Structure
section .multiboot_header
    dd 0xE85250D6    ; Magic
    dd 0             ; Architecture (0 = 32-bit)
    dd header_end - header_start
    ; ... (tags follow)
```

- **Framebuffer Initialization**: GRUB sets up video mode per header request  

---

### **3. Kernel Entry & Setup**

- **Control Transfer**: GRUB jumps to kernel entry point (`KInit`)  
- **Basic CPU State**: Protected mode enabled, interrupts disabled  
- **Stack Setup**: Initialize stack pointer for C code  

```c
// Minimal Kernel Entry (kernel.c)
void KInit(uint32_t magic, uint32_t *mb2_info) {
    // 1. Initialize stack
    // 2. Parse Multiboot2 tags
    // 3. Initialize drivers
}
```

---

### **Boot Flow Diagram**  

```
[Power On] → [BIOS/UEFI] → [GRUB] → [Kernel Header Check]  
               ↓                      ↓  
          [Hardware Init]    [Load Kernel to Memory]  
                                  ↓  
                          [Jump to KInit()]  
                                  ↓  
                   [Initialize Drivers] → [Graphics]  
                                  ↓  
                          [Main Kernel Loop]  
```
