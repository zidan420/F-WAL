; boot/boot.asm
[org 0x7c00]
    KERNEL_OFFSET equ 0x1000

    ; Initialize segments
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x9000     ; Set stack below boot sector

    mov [BOOT_DRIVE], dl

load_kernel:
    mov si, MSG_LOAD_KERNEL
    call print_string       ; Print a message to say we are loading the kernel

    xor ax, ax
    mov es, ax              ; set ES specifically to zero
    mov bx, KERNEL_OFFSET   ; Set-up parameters for our disk_load routine , so
    mov dh, 15              ; that we load the first 15 sectors (excluding
    mov dl, [BOOT_DRIVE]    ; the boot sector) from the boot disk (i.e. our
    call disk_load          ; kernel code ) to address KERNEL_OFFSET

check_kernel_loaded:
    mov si, MSG_KERNEL_OFFSET
    call print_string
    mov ax, [KERNEL_OFFSET]
    call print_hex

switch_pm:
    call switch_to_pm

    jmp $

%include "boot/print/print_string.asm"
%include "boot/disk_load.asm"
%include "boot/print/print_hex.asm"
%include "boot/gdt.asm"
%include "boot/switch_to_pm.asm"

[bits 32]
BEGIN_PM:
    call KERNEL_OFFSET
    jmp $

; -------------------------------------------------
BOOT_DRIVE db 0
MSG db 'Hello World', 0
MSG_LOAD_KERNEL db "Loading kernel" , 0
MSG_KERNEL_OFFSET db "Kernel loaded at offset: 0x", 0

; Padding
times 436-($-$$) db 0
dd 0x00000000
dw 0x0000
times 64 db 0
dw 0xaa55
