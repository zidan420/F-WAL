#ifndef KEYBOARD_H
#define KEYBOARD_H

#define KEYBOARD_BUFFER_SIZE 256  // Arbitrary size; adjust as needed

// functions
void kb_enqueue(unsigned char c);
unsigned char kb_dequeue(void);  // Returns 0 if empty
int kb_has_input(void);  // Non-zero if buffer has data

// Function prototype for the C-level interrupt handler
void keyboard_handler();

// Global variable to track keyboard state (e.g., Shift pressed)
extern int shift_pressed; 
extern int caps_pressed; 

// Scan code constants for modifier keys
#define LSHIFT_MAKE 0x2A
#define LSHIFT_BREAK 0xAA // 0x2A + 0x80
#define RSHIFT_MAKE 0x36
#define RSHIFT_BREAK 0xB6 // 0x36 + 0x80
#define CAPS_MAKE 0x3A

// Basic Keymap (Scan Code Set 1) - Only includes common keys for simplicity
// Index corresponds to the Make Code (up to 0x58)
static const unsigned char QWERTY_MAP[] = {
    0,  0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',  '\t',  // 0x00-0x0F
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0,  'a', 's', // 0x10-0x1F
    'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x', 'c', 'v', // 0x20-0x2F
    'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0 // ... and so on
};

static const unsigned char SHIFTED_MAP[] = {
    0,  0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',  '\t',  // 0x00-0x0F
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', 0,  'A', 'S', // 0x10-0x1F
    'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '\"', '~', 0, '|', 'Z', 'X', 'C', 'V', // 0x20-0x2F
    'B', 'N', 'M', '<', '>', '?', 0, 0, 0, ' ', 0 // ... and so on
};

#endif