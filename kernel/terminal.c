#include "../drivers/graphics.h"
#include "../drivers/screen.h"
#include "../drivers/keyboard.h"

// Define content area boundaries (based on your draw_rect calls)
#define CONTENT_TOP 2
#define CONTENT_BOTTOM 22  // Last row for content (draw_rect height 21 from row 2 → row 22)
#define CONTENT_LEFT 1
#define CONTENT_WIDTH 78  // Cols 1–78 for green bg
#define CONTENT_BORDER_BG LIGHT_BLUE
#define CONTENT_BG GREEN

// Function to scroll the content area (keep header fixed)
void scroll_terminal() {
    // Shift rows 3–22 up to 2–21 (full row copy preserves border attributes)
    for (int row = CONTENT_TOP; row < CONTENT_BOTTOM; row++) {
        memory_copy(
            (char*)(VIDEO_ADDRESS + get_screen_offset(0, row + 1)),
            (char*)(VIDEO_ADDRESS + get_screen_offset(0, row)),
            MAX_COLS * 2  // Full row (borders included)
        );
    }

    // Blank the last content row (22) with proper background colors
    for (int col = 0; col < MAX_COLS; col++) {
        char bg = CONTENT_BORDER_BG;  // Default for borders (cols 0, 79)
        if (col >= CONTENT_LEFT && col < CONTENT_LEFT + CONTENT_WIDTH) {
            bg = CONTENT_BG;  // Green for content area
        }
        unsigned char attr = COLOR(WHITE, bg);
        unsigned char *vidmem = (unsigned char *)(VIDEO_ADDRESS + get_screen_offset(col, CONTENT_BOTTOM));
        *vidmem = ' ';         // Character: space
        *(vidmem + 1) = attr;  // Attribute: white fg on bg
    }

    // Optional: Update cursor if needed, but handled in print calls
}

void draw_terminal() {
    draw_rect(0, 0, 80, 25, LIGHT_BLUE);
    draw_rect(0, 0, 80, 2, BLUE);
    draw_rect(1, 2, 78, 21, GREEN);
    print_at("Terminal", 36, 1, COLOR(WHITE, BLUE));
    print_at(">>> ", 2, 3, COLOR(BLUE, GREEN));

    int char_printed = 0, char_x = 6, char_y = 3;  // Start after initial prompt

    while (1) {
        if (kb_has_input()) {
            unsigned char c = kb_dequeue();
            if (c == '\n') {
                char_printed = 0;
                char_x = 2;
                char_y++;
                // Check for scroll after increment
                if (char_y > CONTENT_BOTTOM) {
                    scroll_terminal();
                    char_y = CONTENT_BOTTOM;
                }
                print_at(">>> ", char_x, char_y, COLOR(BLUE, GREEN));
                char_x += 4;  // Advance past ">>> "
            } else if (c == '\b') {
                if (char_printed > 0) {
                    // Prevent backspacing past initial prompt position
                    if (!(char_y == 3 && char_x <= 6)) {
                        if (--char_x < 2) {  // Wrap to previous line if at start of line
                            char_x = 78 - 1;  // End of previous line (adjust if needed)
                            char_y--;
                            // Prevent going above content top
                            if (char_y < CONTENT_TOP + 1) {
                                char_y = CONTENT_TOP + 1;  // Min row 3
                                char_x = 6;  // Reset to start if over
                            }
                        }
                        char_printed--;
                        // Erase the character
                        print_char(' ', char_x, char_y, COLOR(WHITE, GREEN));
                        set_cursor(get_screen_offset(char_x, char_y));  // Move cursor back
                    }
                }
            } else if (c == '\t') {
                int spaces = 0;
                while (spaces < 4) {
                    print_char(' ', char_x, char_y, COLOR(WHITE, GREEN));
                    char_printed++;
                    if (++char_x == 78) {
                        char_x = 2;
                        char_y++;
                        // Check for scroll after increment
                        if (char_y > CONTENT_BOTTOM) {
                            scroll_terminal();
                            char_y = CONTENT_BOTTOM;
                        }
                    }
                    spaces++;
                }
            } else {
                print_char(c, char_x, char_y, COLOR(WHITE, GREEN));
                char_printed++;
                if (++char_x == 78) {
                    char_x = 2;
                    char_y++;
                    // Check for scroll after increment
                    if (char_y > CONTENT_BOTTOM) {
                        scroll_terminal();
                        char_y = CONTENT_BOTTOM;
                    }
                }
            }
        }
    }
}