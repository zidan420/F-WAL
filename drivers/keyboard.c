/* drivers/keyboard.c */
#include "keyboard.h"
#include "low_level.h"
#include "screen.h"

static int shift_down = 0;

// Check if the keyboard controller has data available
int keyboard_has_key() {
    // Bit 0 of port 0x64 indicates "output buffer full"
    return port_byte_in(KEYBOARD_STATUS_PORT) & 1;
}

// Get character from keyboard (handles shift + basic scancodes)
char keyboard_read_char() {
    /* Wait INDEFINITELY for keyboard input status */
    while (!keyboard_has_key()) { /* Busy wait */ }

    /* Read keyboard input (e.g. a single scancode) */
    unsigned char scancode = port_byte_in(KEYBOARD_DATA_PORT);

    // --- Handle key releases (break codes) ---
    if (scancode & 0x80) {
        // Remove high bit (0x80) to get original make code
        unsigned char make = scancode & 0x7F;

        if (make == LSHIFT_MAKE || make == RSHIFT_MAKE)
            shift_down = 0;

        return 0; // release produces no char
    }

    // --- Handle key presses (make codes) ---
    if (scancode == LSHIFT_MAKE || scancode == RSHIFT_MAKE) {
        shift_down = 1;
        return 0;
    }

    // Check bounds
    if (scancode >= sizeof(QWERTY_MAP))
        return 0;

    // Lookup character
    unsigned char ch = shift_down ? SHIFTED_MAP[scancode] : QWERTY_MAP[scancode];

    return ch;
}
