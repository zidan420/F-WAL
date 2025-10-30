# =========================
# Variables
# =========================
NASM     = nasm
GCC      = gcc
LD       = ld
QEMU     = qemu-system-i386
VBOXMAN  = VBoxManage

# Directories
BOOT_DIR    = boot
KERNEL_DIR  = kernel
DRIVERS_DIR = drivers

# Bootloader
BOOT_SRC = $(BOOT_DIR)/boot.asm
BOOT_BIN = boot.bin

# Kernel
KERNEL_ENTRY = $(KERNEL_DIR)/kernel_entry.asm
KERNEL_SRC   = $(wildcard $(KERNEL_DIR)/*.c)
DRIVER_SRC   = $(wildcard $(DRIVERS_DIR)/*.c)
KERNEL_OBJS  = $(KERNEL_ENTRY:.asm=.o) $(KERNEL_SRC:.c=.o) $(DRIVER_SRC:.c=.o)
KERNEL_BIN   = kernel.bin

# Image
IMG      = boot.img
VDI      = boot.vdi

# Compiler/assembler flags
CFLAGS   = -m32 -ffreestanding -fno-pic -fno-pie -nostdlib -nostartfiles -Wall -Wextra
NASMFLAGS_BOOT = -f bin
NASMFLAGS_ELF  = -f elf32

# =========================
# Targets
# =========================
all: $(VDI)

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
$(IMG): $(BOOT_BIN) $(KERNEL_BIN)
	# Create empty floppy image (1.44MB)
	dd if=/dev/zero of=$@ bs=512 count=2880
	# Copy bootloader
	dd if=$(BOOT_BIN) of=$@ conv=notrunc
	# Copy kernel after first sector
	dd if=$(KERNEL_BIN) of=$@ bs=512 seek=1 conv=notrunc

# -------------------------
# Convert to VDI for VirtualBox
$(VDI): $(IMG)
	$(VBOXMAN) convertdd $(IMG) $@ --format VDI

# -------------------------
# Clean build artifacts
clean:
	rm -f $(BOOT_BIN) $(KERNEL_OBJS) $(KERNEL_BIN) $(IMG) $(VDI)
