ASM = nasm
CC = gcc
LD = ld
GRUB_FILE = grub-mkrescue

# Flags
ASMFLAGS = -f elf32
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
         -nostartfiles -nodefaultlibs -Wall -Wextra -Iheaders
LDFLAGS = -m elf_i386 -T src/linker.ld

# Directories
BUILD_DIR = build
ISO_DIR = build/isofiles
KERNEL = protos-alpha.kernel
ISO = release/protos-alpha.iso
RELEASE_DIR = release/

# Source files
BOOT_SRC = src/boot/boot.asm
KERNEL_SRC = src/kernel/kernel.c
INIT_SRC = src/kernel/init.c
LIB_SRC = src/lib/library.c
DRIVERS_SRC = $(wildcard src/drivers/*.c)
# Object files
BOOT_OBJ = $(BUILD_DIR)/boot/boot.o
KERNEL_OBJ = $(BUILD_DIR)/kernel/kernel.o
INIT_OBJ = $(BUILD_DIR)/kernel/init.o
LIB_OBJ = $(BUILD_DIR)/lib/library.o
DRIVERS_OBJ = $(patsubst src/drivers/%.c,$(BUILD_DIR)/drivers/%.o,$(DRIVERS_SRC))# Object files
.PHONY: all clean run iso

all: $(KERNEL)

$(KERNEL): $(BOOT_OBJ) $(KERNEL_OBJ) $(INIT_OBJ) $(LIB_OBJ) $(DRIVERS_OBJ) 
	@mkdir -p $(BUILD_DIR)
	$(LD) $(LDFLAGS) $^ -o $(BUILD_DIR)/$@

$(BOOT_OBJ): $(BOOT_SRC)
	@mkdir -p $(BUILD_DIR)/boot
	$(ASM) $(ASMFLAGS) $< -o $@

$(INIT_OBJ): $(INIT_SRC)
	@mkdir -p $(BUILD_DIR)/kernel
	$(CC) $(CFLAGS) -c $< -o $@

$(KERNEL_OBJ): $(KERNEL_SRC)
	@mkdir -p $(BUILD_DIR)/kernel
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB_OBJ): $(LIB_SRC)
	@mkdir -p $(BUILD_DIR)/lib
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/drivers/%.o: src/drivers/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

iso: $(KERNEL)
	@mkdir -p $(ISO_DIR)/boot/grub
	mkdir -p $(RELEASE_DIR)
	cp conf/grub.cfg $(ISO_DIR)/boot/grub/
	cp $(BUILD_DIR)/$(KERNEL) $(ISO_DIR)/boot/
	$(GRUB_FILE) -o $(ISO) $(ISO_DIR)

run: iso
	qemu-system-x86_64 -cdrom $(ISO)

clean:
	rm -rf $(BUILD_DIR) $(ISO_DIR) $(ISO)
