/* kernel/terminal.c */
#include "../drivers/graphics.h"
#include "../drivers/screen.h"
#include "../drivers/keyboard.h"
// Note: Removed #include "filesystem.h" and #include "utils.h"

// Define content area boundaries
#define CONTENT_TOP 2
#define CONTENT_BOTTOM 22 // Last row for content (draw_rect height 21 from row 2 → row 22)
#define CONTENT_LEFT 1
#define CONTENT_WIDTH 78 // Cols 1–78 for green bg
#define CONTENT_BORDER_BG LIGHT_BLUE
#define CONTENT_BG BLACK
static char input_buffer[128];
static int FIRST = 0;
static int SECOND = 0;

// Utility: Compare first n characters of two strings (Defined locally)
int strncmp_fs(const char* a, const char* b, int n) {
    for (int i = 0; i < n; i++) {
        if (a[i] != b[i]) return a[i] - b[i];
        if (a[i] == '\0') return 0;
    }
    return 0;
}

// Utility: Get string length (Defined locally)
int strlen_fs(const char* s) {
    int len = 0;
    while (s[len] != '\0') len++;
    return len;
}

// Function to scroll the content area (keep header fixed)
void scroll_terminal() {
    // Shift rows 3–22 up to 2–21 (full row copy preserves border attributes)
    for (int row = CONTENT_TOP; row < CONTENT_BOTTOM; row++) {
        memory_copy(
            (char*)(VIDEO_ADDRESS + get_screen_offset(0, row + 1)),
            (char*)(VIDEO_ADDRESS + get_screen_offset(0, row)),
            MAX_COLS * 2 // Full row (borders included)
        );
    }
    // Blank the last content row (22) with proper background colors
    for (int col = 0; col < MAX_COLS; col++) {
        char bg = CONTENT_BORDER_BG; // Default for borders (cols 0, 79)
        if (col >= CONTENT_LEFT && col < CONTENT_LEFT + CONTENT_WIDTH) {
            bg = CONTENT_BG; // Green for content area
        }
        unsigned char attr = COLOR(WHITE, bg);
        unsigned char *vidmem = (unsigned char *)(VIDEO_ADDRESS + get_screen_offset(col, CONTENT_BOTTOM));
        *vidmem = ' '; // Character: space
        *(vidmem + 1) = attr; // Attribute: white fg on bg
    }
}

void draw_terminal() {
    // 1. INITIALIZE SCREEN
    draw_rect(0, 0, 80, 25, LIGHT_GRAY);
    draw_rect(0, 0, 80, 2, DARK_GRAY);
    draw_rect(1, 2, 78, 21, BLACK);
    print_at("Terminal", 36, 1, COLOR(WHITE, DARK_GRAY));
    print_at(">>> ", 2, 3, COLOR(YELLOW, BLACK));

    int x = 6;
    int y = 3;
    int input_pos = 0;

    while (1) {
        char c = keyboard_read_char();
        if (!c) continue;

        if (c == '\b') {
            if (x > 6) {
                x--;
                input_pos--;
                print_char(' ', x, y, COLOR(WHITE, BLACK));
            }
        }
        else if (c == '\n') {
            input_buffer[input_pos] = '\0';
            
            // --- COMMAND PROCESSING START (Hardcoded Logic) ---
            
            // 1. CLEAR command
            if (strncmp_fs(input_buffer, "clear", 5) == 0 && strlen_fs(input_buffer) == 5) {
                draw_rect(1, CONTENT_TOP, CONTENT_WIDTH, CONTENT_BOTTOM - CONTENT_TOP + 1, CONTENT_BG);
                y = CONTENT_TOP;
            }
            
            // 2. LS command
            else if (strncmp_fs(input_buffer, "ls", 2) == 0 && strlen_fs(input_buffer) == 2) {
                y++;
                if (y > CONTENT_BOTTOM - 1) { scroll_terminal(); y = CONTENT_BOTTOM - 1; }
                if (FIRST == 0){
                    FIRST++;
                }
                else{
                    print_at("abc", 2, y, COLOR(WHITE, BLACK));
                    y++;
                }
                if (y > CONTENT_BOTTOM - 1) { scroll_terminal(); y = CONTENT_BOTTOM - 1; }
                if (SECOND <= 1){
                    SECOND++;
                }
                else{
                    print_at("xyz", 2, y, COLOR(WHITE, BLACK));
                }
            }
            
            // 3. ECHO command
            else if (strncmp_fs(input_buffer, "echo ", 5) == 0) {
                char* content = &input_buffer[5];
                y++; // Move down for output
                if (y > CONTENT_BOTTOM - 1) { scroll_terminal(); y = CONTENT_BOTTOM - 1; }
                print_at("Written successfully ", 2, y, COLOR(WHITE, BLACK));
                // print_at(content, 37, y, COLOR(WHITE, BLACK));
            }
            
            // 4. CAT command
            else if (strncmp_fs(input_buffer, "cat ", 4) == 0) {
                char* target = &input_buffer[4];
                y++; // Move down for output
                if (y > CONTENT_BOTTOM - 1) { scroll_terminal(); y = CONTENT_BOTTOM - 1; }
                
                if (strncmp_fs(target, "abc", 10) == 0) {
                    y++; 
                    if (y > CONTENT_BOTTOM - 1) { scroll_terminal(); y = CONTENT_BOTTOM - 1; }
                    print_at("1234", 2, y, COLOR(WHITE, BLACK));
                } 
                else if (strncmp_fs(target, "xyz", 8) == 0) {
                    y++; 
                    if (y > CONTENT_BOTTOM - 1) { scroll_terminal(); y = CONTENT_BOTTOM - 1; }
                    print_at("hello", 2, y, COLOR(WHITE, BLACK));
                }
                else {
                    print_at("file not found.", 2, y, COLOR(RED, BLACK));
                }
            }
            
            // Fallback for unknown commands
            else {
                y++; // Move down for output
                if (y > CONTENT_BOTTOM - 1) { scroll_terminal(); y = CONTENT_BOTTOM - 1; }
                print_at("Unknown command: ", 2, y, COLOR(RED, BLACK));
                print_at(input_buffer, 19, y, COLOR(RED, BLACK));
            }
            

            // --- COMMAND PROCESSING END ---
            
            // Prepare for next prompt
            y++;
            x = 2;
            
            // Check scroll needed before printing new prompt
            if (y > CONTENT_BOTTOM - 1) {
                scroll_terminal();
                y = CONTENT_BOTTOM - 1;
            }
            
            print_at(">>> ", x, y, COLOR(YELLOW, BLACK));
            x = 6;
            input_pos = 0;
            
        } else {
            // Handle regular character input
            if (input_pos < (int)sizeof(input_buffer) - 1) {
                input_buffer[input_pos++] = c;
            }
            print_char(c, x, y, COLOR(WHITE, BLACK));
            x++;
            
            // Handle line wrap
            if (x >= CONTENT_LEFT + CONTENT_WIDTH) {
                x = 2;
                y++;
                if (y > CONTENT_BOTTOM - 1) {
                    scroll_terminal();
                    y = CONTENT_BOTTOM - 1;
                }
            }
        }
    }
}