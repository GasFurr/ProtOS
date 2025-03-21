CC = gcc
NASM = nasm
LD = ld
GRUB = grub-mkrescue
MKDIR = mkdir -p

SRC_DIR = src
BUILD_DIR = build
DRIVERS_DIR = $(SRC_DIR)/drivers

SOURCES = \
	$(SRC_DIR)/boot.asm \
	$(SRC_DIR)/gdt.asm \
	$(SRC_DIR)/kernel.c \
	$(SRC_DIR)/idt.asm \
	$(SRC_DIR)/commands.c\
	$(DRIVERS_DIR)/timer.c \
	$(DRIVERS_DIR)/keyboard.c


OBJECTS = \
	$(BUILD_DIR)/boot.o \
	$(BUILD_DIR)/gdt.o \
	$(BUILD_DIR)/commands.o\
	$(BUILD_DIR)/kernel.o \
	$(BUILD_DIR)/idt.o \
	$(BUILD_DIR)/drivers/timer.o \
	$(BUILD_DIR)/drivers/keyboard.o


ASMFLAGS = -f elf32
CFLAGS = -m32 -ffreestanding -nostdlib -Wall -Wextra \
         -fno-stack-protector -mno-red-zone \
         -fno-pie -fno-pic -I$(SRC_DIR)
LDFLAGS = -m elf_i386 -T $(SRC_DIR)/linker.ld

KERNEL = $(BUILD_DIR)/mykernel.bin
ISO = $(BUILD_DIR)/myos.iso
ISODIR = $(BUILD_DIR)/isodir
BOOTDIR = $(ISODIR)/boot
GRUBDIR = $(BOOTDIR)/grub

.PHONY: all clean run

all: $(ISO)

$(ISO): $(KERNEL)
	@$(MKDIR) $(GRUBDIR)
	cp $(KERNEL) $(BOOTDIR)
	echo 'menuentry "MyOS" { multiboot /boot/mykernel.bin; boot }' > $(GRUBDIR)/grub.cfg
	$(GRUB) -o $@ $(ISODIR)

$(KERNEL): $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.asm
	@$(MKDIR) $(@D)
	$(NASM) $(ASMFLAGS) $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@$(MKDIR) $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/drivers/%.o: $(DRIVERS_DIR)/%.c
	@$(MKDIR) $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

run: $(ISO)
	qemu-system-i386 -cdrom $(ISO)

debug: $(ISO)
	qemu-system-i386 -cdrom $(ISO) -d int no-reboot
