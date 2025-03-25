ASM = nasm
CC = gcc
LD = ld
GRUB_FILE = grub-mkrescue

# Flags
ASMFLAGS = -f elf32
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
         -nostartfiles -nodefaultlibs -Wall -Wextra -Iheaders/
LDFLAGS = -m elf_i386 -T src/linker.ld

# Directories
BUILD_DIR = build
ISO_DIR = build/isofiles
KERNEL = myos.kernel
ISO = myos.iso

# Source files
BOOT_SRC = src/boot/boot.asm
KERNEL_SRC = src/kernel/kernel.c
LIB_SRC = src/lib/library.c

# Object files
BOOT_OBJ = $(BUILD_DIR)/boot.o
KERNEL_OBJ = $(BUILD_DIR)/kernel.o
LIB_OBJ = $(BUILD_DIR)/library.o

.PHONY: all clean run iso

all: $(KERNEL)

$(KERNEL): $(BOOT_OBJ) $(KERNEL_OBJ) $(LIB_OBJ)
	@mkdir -p $(BUILD_DIR)
	$(LD) $(LDFLAGS) $^ -o $(BUILD_DIR)/$@

$(BOOT_OBJ): $(BOOT_SRC)
	@mkdir -p $(BUILD_DIR)
	$(ASM) $(ASMFLAGS) $< -o $@

$(KERNEL_OBJ): $(KERNEL_SRC)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB_OBJ): $(LIB_SRC)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

iso: $(KERNEL)
	@mkdir -p $(ISO_DIR)/boot/grub
	cp conf/grub.cfg $(ISO_DIR)/boot/grub/
	cp $(BUILD_DIR)/$(KERNEL) $(ISO_DIR)/boot/
	$(GRUB_FILE) -o $(ISO) $(ISO_DIR)

run: iso
	qemu-system-x86_64 -cdrom $(ISO)

clean:
	rm -rf $(BUILD_DIR) $(ISO_DIR) $(ISO)
