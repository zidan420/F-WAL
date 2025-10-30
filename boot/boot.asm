; A boot sector that loads the kernel, enters the 32-bit protected mode and then executes the kernel
[org 0x7c00]
KERNEL_OFFSET equ 0x1000 	; This is the memory offset to which we will load our kernel

	mov [BOOT_DRIVE], dl	; BIOS stores our boot drive in DL , so it ’s
							; best to remember this for later.
	mov bp , 0x9000		 	; Set the stack.
	mov sp , bp
	mov si , MSG_REAL_MODE 	; Announce that we are starting
	call print_string 		; booting from 16 - bit real mode

	call load_kernel 		; Load kernel
	
	call switch_to_pm 		; Switch to Protected mode from where we will never return

	jmp $

%include "boot/print/print_string.asm"
%include "boot/disk_load.asm"
%include "boot/gdt.asm"
%include "drivers/print_string_pm.asm"
%include "boot/switch_to_pm.asm"


[bits 16]

load_kernel:
	mov si, MSG_LOAD_KERNEL
	call print_string 		; Print a message to say we are loading the kernel

	mov bx, KERNEL_OFFSET 	; Set-up parameters for our disk_load routine , so
	mov dh, 15 				; that we load the first 15 sectors (excluding
	mov dl, [ BOOT_DRIVE ] 	; the boot sector) from the boot disk (i.e. our
	call disk_load 			; kernel code ) to address KERNEL_OFFSET
	ret

[ bits 32]
; This is where we arrive after switching to and initialising protected mode.
BEGIN_PM :
	mov ebx , MSG_PROT_MODE
	call print_string_pm 		; Use our 32 - bit print routine.

	call KERNEL_OFFSET 			; Execute the Kernel

	jmp $ 						; Hang.

; Global variables
BOOT_DRIVE db 0
MSG_REAL_MODE db "Started in 16 - bit Real Mode" , 0
MSG_PROT_MODE db "Successfully landed in 32 - bit Protected Mode" , 0
MSG_LOAD_KERNEL db "Loading kernel into memory." , 0

; Bootsector padding
times 510-($-$$) db 0
dw 0xaa55
