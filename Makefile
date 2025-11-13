# =========================
# Variables
# =========================
NASM     = nasm
GCC      = gcc
LD       = ld
QEMU     = qemu-system-i386

# Directories
BOOT_DIR    = boot
KERNEL_DIR  = kernel
DRIVERS_DIR = drivers

# Bootloader
BOOT_SRC = $(BOOT_DIR)/boot.asm
BOOT_BIN = boot.bin

# Kernel
KERNEL_ASM   = $(wildcard $(KERNEL_DIR)/*.asm)
KERNEL_SRC   = $(wildcard $(KERNEL_DIR)/*.c)
DRIVER_SRC   = $(wildcard $(DRIVERS_DIR)/*.c)
KERNEL_OBJS  = $(KERNEL_ASM:.asm=.o) $(KERNEL_SRC:.c=.o) $(DRIVER_SRC:.c=.o)
KERNEL_BIN   = kernel.bin

# Image
USB      = /dev/sdb

# Compiler/assembler flags
CFLAGS   = -m32 -ffreestanding -fno-pic -fno-pie -nostdlib -nostartfiles -Wall -Wextra
NASMFLAGS_BOOT = -f bin
NASMFLAGS_ELF  = -f elf32

# =========================
# Targets
# =========================
all: $(USB)

# -------------------------
# Assemble bootloader
$(BOOT_BIN): $(BOOT_SRC)
	$(NASM) $(NASMFLAGS_BOOT) $< -o $@

# -------------------------
# Compile kernel & drivers
%.o: %.c
	$(GCC) $(CFLAGS) -c $< -o $@

%.o: %.asm
	$(NASM) $(NASMFLAGS_ELF) $< -o $@

# -------------------------
# Link kernel
$(KERNEL_BIN): $(KERNEL_OBJS)
	$(LD) -m elf_i386 -Ttext 0x1000 $^ -o $@ --oformat binary

# -------------------------
# Create floppy image and copy bootloader + kernel
$(USB): $(BOOT_BIN) $(KERNEL_BIN)
	# Copy Bootloader to first 436 bytes so as not to overwrite partition
	dd if=$(BOOT_BIN) of=$@ bs=1 count=436 conv=notrunc
	# Copy kernel after first sector
	dd if=$(KERNEL_BIN) of=$@ bs=512 seek=1 conv=notrunc status=progress

# -------------------------
partition:
	sudo dd if=/dev/zero of=$(USB) bs=512 count=1000 conv=notrunc status=progress
	sudo fdisk $(USB) <<EOF
		o
		n
		p
		1
		2048

		a
		w
		EOF
	sudo mkfs.vfat -F 32 $(USB)1
	@sync

# -------------------------
# Clean build artifacts
clean:
	rm -f $(BOOT_BIN) $(KERNEL_OBJS) $(KERNEL_BIN)
