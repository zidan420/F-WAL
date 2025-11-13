; kernel/keyboard_asm.asm
section .text

; C function prototype: void keyboard_handler();
extern keyboard_handler

; The assembly entry point for IRQ 1 (Interrupt Vector 0x21)
global _keyboard_asm_handler
_keyboard_asm_handler:
    ; 1. Save all general-purpose registers (context switch)
    pusha

    ; 2. Call the C-language handler
    call keyboard_handler

    ; 3. Restore registers and return from interrupt
    popa
    iret ; This instruction returns from an interrupt
