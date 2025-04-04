# Building ProtOS

**ProtOS Build Documentation**  
A simple guide to build and run your OS project using the provided Makefile.

---

### **Prerequisites**  

### Summon dependencies (Two basic disto's, i use arch btw)

Arch/Manjaro (yay required)

```bash
# Summon compiler dragons (i use arch btw)
yay -S grub nasm gcc binutils qemu-full edk2-ovmf
```

(Didn't tested, but in theory this should work)
Debian/Ubuntu (apt required)

```bash
# For our Debian/Ubuntu friends:
sudo apt install grub-pc nasm gcc binutils qemu-system-x86 ovmf
```

### UEFI

> QEMU testing with UEFI requires OVMF firmware.
> We don't ship it because of the license.
> [EDK2](https://github.com/tianocore/edk2) or `sudo pacman -S edk2-ovmf`
> then copy `OVMF_CODE.fd` to firmware folder.
> And don't forget about permissions.

---

### **Key Components**  

#### **1. Directories**  

- `src/`: Contains all C and assembly (.asm) source files.  
- `headers/`: Header files for the project.  
- `build/`: Output directory for compiled objects and the final kernel.  
- `grub/`: GRUB configuration (`grub.cfg`).  
- `isodir/`: Temporary folder for ISO creation.  
- `firmware/`: Holds UEFI firmware files (e.g., `OVMF_CODE.4m.fd`).  

#### **2. Important Files**  

- `linker.ld`: Linker script (ensures proper kernel layout).  
- `grub.cfg`: GRUB bootloader configuration.  

---

### **Build Process**  

#### **Step 1: Compile Sources**  

- **C Files**: Compiled into `.o` files with `gcc`.  
- **Assembly Files**: Assembled into `.o` files with `nasm`.  

#### **Step 2: Link the Kernel**  

- All `.o` files are linked using `ld` and the `linker.ld` script to create `protos.kernel`.  

#### **Step 3: Create Bootable ISO**  

- The kernel and GRUB files are bundled into `ProtOS.iso` using `grub-mkrescue`.  

---

### **Makefile Commands**  

Run these from the project root:  

| Command         | Action                                                                 |  
|-----------------|-----------------------------------------------------------------------|  
| `make all`      | Compiles the kernel (does not create ISO).                            |  
| `make`          | Same as `make all`.                                                   |  
| `make bios`     | Runs the OS in QEMU using BIOS emulation.                             |  
| `make run`      | Runs the OS in QEMU with UEFI firmware.                               |  
| `make debug`    | Runs with debug options and logs output to `uefi_debug.log`.          |  
| `make release`  | Copies the ISO to the `release/` directory.                           |  
| `make clean`    | Deletes build artifacts (`build/`, `isodir/`, etc.).                  |  

---

### **Example Workflow**  

1. **Build the Kernel and ISO**:  

   ```bash  
   make all   # Compiles the kernel  
   make       # Shortcut for the above  
   ```  

2. **Run in QEMU (UEFI)**:  

   ```bash  
   make run  
   ```  

3. **Run in QEMU (BIOS)**:  

   ```bash  
   make bios  
   ```  
