; load DH sectors to ES : BX from drive DL
; depends on "print_string.asm"
; Assumes C = 0, H = 0, S = 2 (because bootloader is already loaded in Sector 1)
; specify ES:BX to tell where to load the disk (sectors)

disk_load:
    push dx         ; Store DX on stack so later we can recall
                    ; how many sectors were request to be read ,
                    ; even if it is altered in the meantime
    mov ah, 0x02    ; BIOS read sector function
    mov al, dh      ; Read dh sectors
    mov ch, 0x00    ; Select cylinder 0
    mov dh, 0x00    ; Select head 0
    mov cl, 0x02    ; Start reading from second sector ( i.e.
                    ; after the boot sector )

    int 0x13        ; BIOS interrupt

    jc .disk_error1     ; Jump if error ( i.e. carry flag set )

    pop dx              ; Restore dx from stack
    cmp dh, al          ; if AL ( sectors read ) != DH ( sectors expected )
    jne .disk_error2    ; display error message
    ret

.disk_error1:
    mov si, DISK_ERROR_HARDWARE
    call print_string
    jmp $

.disk_error2:
    mov si, DISK_ERROR_LOGICAL
    call print_string
    jmp $

DISK_ERROR_HARDWARE:
    db "Disk read error - Hardware failure", 0

DISK_ERROR_LOGICAL:
    db "Disk read error - Logical failure", 0