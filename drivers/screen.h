#ifndef SCREEN_H
#define SCREEN_H

#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25			// resolution -> 640x400
#define MAX_COLS 80 		// width of each char = 8 and height of each char = 16
#define BYTES_PER_CHAR 2

// Screen device I/O ports
#define REG_SCREEN_CTRL 0x3D4
#define REG_SCREEN_DATA 0x3D5

// Function prototypes
int get_screen_offset(int col, int row);
int get_cursor();
void set_cursor(int offset);
void memory_copy(char* source, char* dest, int no_bytes);
int handle_scrolling(int cursor_offset);
void print_char(char character, int col, int row, char attribute_byte);
void print_at(char* message, int col, int row, char attribute_byte);
void clear_screen();

#endif
