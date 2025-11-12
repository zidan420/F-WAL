#include "../drivers/graphics.h"
#include "../drivers/screen.h"

void draw_terminal(){
	draw_rect(0, 0, 80, 25, LIGHT_BLUE);
	draw_rect(0, 0, 80, 2, BLUE);
	draw_rect(1, 2, 78, 21, GREEN);
	print_at("Terminal", 36, 1, COLOR(WHITE, BLUE));
	print_at(">>>", 2, 3, COLOR(BLUE, GREEN));
}
