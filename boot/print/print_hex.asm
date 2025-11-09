; print_hex: print AX as hex (most significant first)
print_hex:
    pusha
    mov cx, 4
    mov bx, ax

.hex_loop:
    mov ax, bx
    shr ax, 12          ; move highest nibble to low bits
    and al, 0x0F
    cmp al, 9
    jbe .digit
    add al, 'A' - 10
    jmp .print
.digit:
    add al, '0'
.print:
    mov ah, 0x0E
    int 0x10

    shl bx, 4           ; move next nibble into position
    loop .hex_loop
    popa
    ret
