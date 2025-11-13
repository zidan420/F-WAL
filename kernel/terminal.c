#include "../drivers/graphics.h"
#include "../drivers/screen.h"
#include "../drivers/keyboard.h"

void draw_terminal(){
	draw_rect(0, 0, 80, 25, LIGHT_BLUE);
	draw_rect(0, 0, 80, 2, BLUE);
	draw_rect(1, 2, 78, 21, GREEN);
	print_at("Terminal", 36, 1, COLOR(WHITE, BLUE));
	print_at(">>> ", 2, 3, COLOR(BLUE, GREEN));

	int char_printed = 0, char_x = 6, char_y = 3;
	while (1) {
        if (kb_has_input()) {
            unsigned char c = kb_dequeue();

            if (c == '\n') {
            	char_printed = 0;
            	char_x = 2;
            	char_y++;
                // Advance to new line, process command, etc.
                // print_char('\n', -1, -1, COLOR(WHITE, GREEN));
                // print_char(' ', -1, -1, COLOR(WHITE, LIGHT_BLUE));
                print_at(">>> ", char_x, char_y, COLOR(BLUE, GREEN));
                char_x += 4;
            } 
            else if (c == '\b') {
                if (char_printed > 0) {
                	if (--char_x == 1){
                		char_x = 77;
                		char_y--;
                	}
                	char_printed--;
                    print_char(' ', char_x, char_y, COLOR(WHITE, GREEN));
                    set_cursor(get_cursor()-2);
                }
            } 
            else if (c == '\t'){
            	int spaces = 0;
            	while (spaces < 4){
	                print_char(' ', char_x, char_y, COLOR(WHITE, GREEN));
	                char_printed++;
	            	if (++char_x == 78){
	                	char_x = 2;
	                	char_y++;
	                }
	                spaces++;
            	}
            }
            else {
                print_char(c, char_x, char_y, COLOR(WHITE, GREEN));
                char_printed++;
            	if (++char_x == 78){
                	char_x = 2;
                	char_y++;
                }
            }
        }
    }
}
