; put the hex in AX to print

print_hex:
    pusha

    mov cx, 4               ; 4 hex digits for AX (16 bits)
.hex_loop:
    rol ax, 4               ; rotate top nibble into low 4 bits
    mov bl, al
    and bl, 0x0F            ; isolate that nibble
    cmp bl, 9
    jbe .digit
    add bl, 'A' - 10        ; A..F
    jmp .out
.digit:
    add bl, '0'             ; 0..9
.out:
    mov ah, 0x0E            ; BIOS teletype print
    int 0x10

    loop .hex_loop

    popa
    ret
