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
				-I$(Headers)
ASMFflags = -f elf32
LDFlags = -m elf_i386 -T src/linker.ld -nostdlib

# Directories
SourceDir = src
BuildDir = build
GrubConf = grub/grub.cfg
Headers = headers
IsoDir = isodir

# Sources & Objects
CSources := $(shell find $(SourceDir) -type f -name '*.c')
AssemblySrc := $(shell find $(SourceDir) -type f -name '*.asm')
Objects := $(patsubst $(SourceDir)/%.c,$(BuildDir)/%.o,$(CSources)) \
           $(patsubst $(SourceDir)/%.asm,$(BuildDir)/%.o,$(AsemblySrc))

# Outputs
KernelBin = $(BuildDir)/protos.kernel
Iso = release/ProtOS.iso
Release = release

# Commands
.PHONY: all clean run debug release

all: $(KernelBin)
# ISO creation.
$(Iso): $(KernelBin)
	@$(New) $(IsoDir)/boot/grub
	cp $(KernelBin) $(IsoDir)/boot
	cp $(GrubConf) $(IsoDir)/boot/grub
	grub-mkrescue -o $@ $(IsoDir)

# Linking binary.
$(KernelBin): $(Objects)
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
 
run: $(Iso)
	qemu-system-x86_64 -cdrom $(Iso)

release: $(Iso)

clean:
	$(Del) $(BuildDir) $(IsoDir) $(Iso) $(Release)/*
