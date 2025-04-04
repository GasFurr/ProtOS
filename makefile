# Makefile. One of the first thing i need to work on. 

# Essentials
Asm = nasm
CC = gcc
LD = ld
Del = rm -rf
New = mkdir -p

# Flags
CFlags = -m32 -ffreestanding -nostdlib \
				-nostartfiles -Wall -Wextra \
				-I$(Headers) -fno-pie -fno-pic \

ASMFflags = -f elf32
LDFlags = -m elf_i386 -T src/linker.ld -nostdlib

# Directories
SourceDir = src
BuildDir = build
GrubConf = grub/grub.cfg
Headers = headers
IsoDir = isodir
BOOT = build/boot/boot.o


# Sources & Objects
CSources := $(shell find $(SourceDir) -type f -name '*.c')
AssemblySrc := $(shell find $(SourceDir) -type f -name '*.asm')
Objects := $(patsubst $(SourceDir)/%.c,$(BuildDir)/%.o,$(CSources)) \
           $(patsubst $(SourceDir)/%.asm,$(BuildDir)/%.o,$(AsemblySrc))

# Outputs
KernelBin = $(BuildDir)/protos.kernel
Iso = $(BuildDir)/ProtOS.iso
Release = release

# Commands
.PHONY: all clean run debug bios release

all: $(KernelBin)
# ISO creation.
$(Iso): $(KernelBin)
	@$(New) $(IsoDir)/boot/grub
	cp $(KernelBin) $(IsoDir)/boot
	cp $(GrubConf) $(IsoDir)/boot/grub
	grub-mkrescue -o $@ $(IsoDir)

# Linking binary.
$(KernelBin): $(BOOT) $(Objects)
	$(New) $(@D)
	$(LD) $(LDFlags) -o $@ $^

# C Source compilation
$(BuildDir)/%.o: $(SourceDir)/%.c
	@$(New) $(@D)
	$(CC) $(CFlags) -c $< -o $@

# Assembly source compilation
$(BuildDir)/%.o: $(SourceDir)/%.asm
	@$(New) $(@D)
	$(Asm) $(ASMFflags) $< -o $@

bios: $(Iso)
	qemu-system-x86_64 -cdrom $(Iso)

debug: $(Iso)
	qemu-system-x86_64 \
		-machine q35,accel=kvm \
		-cpu host \
		-drive if=pflash,format=raw,readonly=on,file=firmware/OVMF_CODE.4m.fd \
		-drive id=cd,file=$(Iso),format=raw,if=none,media=cdrom \
		-device ide-cd,drive=cd \
		-m 2G \
		-serial stdio \
		-no-reboot \
		-debugcon file:uefi_debug.log \
		-global isa-debugcon.iobase=0x402

run: $(Iso)
	qemu-system-x86_64 \
		-machine q35,accel=kvm \
		-cpu host \
		-drive if=pflash,format=raw,readonly=on,file=firmware/OVMF_CODE.4m.fd \
		-drive id=cd,file=$(Iso),format=raw,if=none,media=cdrom \
		-device ide-cd,drive=cd \
		-m 2G \
		-serial stdio \

release: $(Iso)
	cp $(Iso) $(Release)

clean:
	$(Del) $(BuildDir) $(IsoDir) $(Iso) $(Release)/*
