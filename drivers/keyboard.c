#include "keyboard.h"
#include "low_level.h"
#include "screen.h"
#include "graphics.h"

#define KEYBOARD_DATA_PORT 0x60

// EOI function defined in isr.c
void pic_eoi(unsigned char irq); 

// Global state variable defined in kernel/kernel.c
int shift_pressed = 0; 

/**
 * @brief Handles the keyboard interrupt (IRQ 1).
 */
void keyboard_handler() {
    unsigned char scan_code = port_byte_in(KEYBOARD_DATA_PORT);

    // 1. Handle Modifier Keys (Shift)
    if (scan_code == LSHIFT_MAKE || scan_code == RSHIFT_MAKE) {
        shift_pressed = 1;
    } else if (scan_code == LSHIFT_BREAK || scan_code == RSHIFT_BREAK) {
        shift_pressed = 0;
    } 
    
    // 2. Check if it's a Make Code (Key Press)
    // A break code has the 0x80 bit set, so a make code is less than 0x80
    else if (scan_code < 0x80) {
        
        unsigned char character = 0;

        // Use the appropriate keymap based on the Shift state
        if (shift_pressed) {
            // Ensure scan_code is within bounds of the SHIFTED_MAP
            if (scan_code < sizeof(SHIFTED_MAP)) {
                 character = SHIFTED_MAP[scan_code];
            }
        } else {
            // Ensure scan_code is within bounds of the QWERTY_MAP
            if (scan_code < sizeof(QWERTY_MAP)) {
                character = QWERTY_MAP[scan_code];
            }
        }

        // 3. Print the translated character
        if (character != 0) {
            // Print the actual character
            print_char(character, -1, -1, COLOR(WHITE, GREEN)); 
        } else {
             // Print a placeholder for unmapped or special keys (like F-keys)
             // print_char('?', -1, -1, COLOR(RED, BLACK));
        }
    } 
    // We ignore Break Codes for non-modifier keys for simplicity.
    
    // Acknowledge the interrupt to the PIC (IRQ 1)
    pic_eoi(1);
}