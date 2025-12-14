/* kernel/terminal.c */
#include "../drivers/graphics.h"
#include "../drivers/screen.h"
#include "../drivers/keyboard.h"
#include "filesystem.h"

// Define content area boundaries
#define CONTENT_TOP 2
#define CONTENT_BOTTOM 22  // Last row for content (draw_rect height 21 from row 2 → row 22)
#define CONTENT_LEFT 1
#define CONTENT_WIDTH 78  // Cols 1–78 for green bg
#define CONTENT_BORDER_BG LIGHT_BLUE
#define CONTENT_BG GREEN

static char input_buffer[128];
static char filename[MAX_FILENAME];

// Compare first n characters of two strings
int strncmp_fs(const char* a, const char* b, int n) {
    for (int i = 0; i < n; i++) {
        if (a[i] != b[i]) return a[i] - b[i];
        if (a[i] == '\0') return 0;
    }
    return 0;
}

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
    fs_init();
    draw_rect(0, 0, 80, 25, LIGHT_BLUE);
    draw_rect(0, 0, 80, 2, BLUE);
    draw_rect(1, 2, 78, 21, GREEN);
    print_at("Terminal", 36, 1, COLOR(WHITE, BLUE));
    print_at(">>> ", 2, 3, COLOR(BLUE, GREEN));

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
                print_char(' ', x, y, COLOR(WHITE, GREEN));
            }
        } 
        else if (c == '\n') {
            input_buffer[input_pos] = '\0';

            // Simple RAM FS test: ls command
            if (strncmp_fs(input_buffer, "ls", 2) == 0) {
                int count = fs_get_file_count();
                int line_y = y + 1; // start printing just below the current prompt
                if (count == 0) {
                    print_at("No files found", 2, line_y, COLOR(WHITE, GREEN));
                    line_y++;
                } else {
                    for (int i = 0; i < count; i++) {
                        File* f = fs_get_file_by_index(i);
                        print_at(f->name, 2, line_y++, COLOR(WHITE, GREEN)); // increment after each file
                    }
                }
                y = line_y;  // move prompt down to after output
            }
            // RAM FS test: echo command
            else if (strncmp_fs(input_buffer, "echo ", 5) == 0) {
                // Find filename (next word)
                int i = 5; // skip "echo "
                int fi = 0;

                // Extract filename until space or end
                while (input_buffer[i] != ' ' && input_buffer[i] != '\0' && fi < MAX_FILENAME - 1) {
                    filename[fi++] = input_buffer[i++];
                }
                filename[fi] = '\0';

                // Skip space to content
                while (input_buffer[i] == ' ') i++;
                char* content = &input_buffer[i];

                // Create or get file
                File* f = fs_get_file(filename);
                if (!f) f = fs_create_file(filename);
                fs_write_file(f, content, strlen_fs(content));

                // Optional feedback
                print_at("File written", 2, y + 1, COLOR(WHITE, GREEN));
                y++; // advance cursor for prompt
            }
            // RAM FS test: cat command

            else if (strncmp_fs(input_buffer, "cat ", 4) == 0) {

                int fi = 0, i = 4;

                

                // Extract filename (Keep this logic)

                while (input_buffer[i] != ' ' && input_buffer[i] != '\0' && fi < MAX_FILENAME - 1) {

                    filename[fi++] = input_buffer[i++];

                }

                filename[fi] = '\0';

                

                File* f = fs_get_file(filename);

                

                y++;  // Move down for output

                if (f == 0) {

                    print_at("File not found", 2, y, COLOR(WHITE, GREEN));

                } else {

                    print_at("File found (size: ", 2, y, COLOR(WHITE, GREEN));

                    // Simple size printing (optional, could be complex without number to string function)

                    // If you don't have a simple number to string:

                    // print_at("File found.", 2, y, COLOR(WHITE, GREEN));

                }

            } // Keep this closing brace!



            y++;
            x = 2;
            print_at(">>> ", x, y, COLOR(BLUE, GREEN));
            x = 6;
            input_pos = 0;

            if (y > CONTENT_BOTTOM - 1) {
                scroll_terminal();
                y = CONTENT_BOTTOM - 1;
            }

        } else {
            if (input_pos < (int)sizeof(input_buffer) - 1) {
                input_buffer[input_pos++] = c;
            }
            print_char(c, x, y, COLOR(WHITE, GREEN));
            x++;
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
