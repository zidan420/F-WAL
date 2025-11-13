#include "keyboard.h"
#include "low_level.h"
#include "screen.h"
#include "graphics.h"

#define KEYBOARD_DATA_PORT 0x60

// EOI function defined in isr.c
void pic_eoi(unsigned char irq); 

// Global state variable defined in kernel/kernel.c
int shift_pressed = 0;
int caps_lock = 0;
unsigned char keyboard_buffer[KEYBOARD_BUFFER_SIZE];
int kb_buffer_head = 0;
int kb_buffer_tail = 0;
int kb_buffer_count = 0;

/**
 * @brief Handles the keyboard interrupt (IRQ 1).
 */
void keyboard_handler() {
    unsigned char scan_code = port_byte_in(KEYBOARD_DATA_PORT);

    // Handle modifiers
    if (scan_code == LSHIFT_MAKE || scan_code == RSHIFT_MAKE) {
        shift_pressed = 1;
    } else if (scan_code == LSHIFT_BREAK || scan_code == RSHIFT_BREAK) {
        shift_pressed = 0;
    } else if (scan_code == CAPS_MAKE) {
        caps_lock = !caps_lock;
    } else if (scan_code < 0x80) {
        unsigned char character = 0;
        if (scan_code < 128) {
            // Get base from shift state (handles symbols correctly)
            character = shift_pressed ? SHIFTED_MAP[scan_code] : QWERTY_MAP[scan_code];
        }
        if (character == 0) {
            pic_eoi(1);
            return;  // Unmapped
        }

        // For letters only: Apply caps (XOR with shift)
        if (character >= 'a' && character <= 'z') {
            int effective_case = shift_pressed ^ caps_lock;
            if (effective_case) {
                character -= 32;  // To 'A'-'Z'
            }
        } else if (character >= 'A' && character <= 'Z') {
            int effective_case = shift_pressed ^ caps_lock;
            if (!effective_case) {
                character += 32;  // To 'a'-'z' (if somehow uppercase in base)
            }
        }

        kb_enqueue(character);
    }
    pic_eoi(1);
}

void kb_enqueue(unsigned char c) {
    if (kb_buffer_count >= KEYBOARD_BUFFER_SIZE) {
        // Buffer full: Drop oldest (or handle overflow, e.g., beep)
        kb_buffer_tail = (kb_buffer_tail + 1) % KEYBOARD_BUFFER_SIZE;
    } else {
        kb_buffer_count++;
    }
    keyboard_buffer[kb_buffer_head] = c;
    kb_buffer_head = (kb_buffer_head + 1) % KEYBOARD_BUFFER_SIZE;
}

unsigned char kb_dequeue(void) {
    if (kb_buffer_count == 0) {
        return 0;  // Empty
    }
    unsigned char c = keyboard_buffer[kb_buffer_tail];
    kb_buffer_tail = (kb_buffer_tail + 1) % KEYBOARD_BUFFER_SIZE;
    kb_buffer_count--;
    return c;
}

int kb_has_input(void) {
    return kb_buffer_count > 0;
}