/* kernel/logo.c */
#include "../drivers/graphics.h"
#include "../drivers/screen.h"
#include "../drivers/timer.h"

void draw_logo(int x, int y){
	int scale = 2;
	int width = 1;
	int height = 1;
	int orig_x = x, orig_y = y;
	char fade_steps[4] = {
	    WHITE,
	    LIGHT_GRAY,
	    LIGHT_BLUE,
	    BLUE
	};

	for(int i = 0; i < 4; i++){
		char color = fade_steps[i];

		clear_screen();
		x = orig_x, y = orig_y;
		// draw rectangle
		draw_rect(x, y, 25*width*scale, 9*height, WHITE);
		draw_rect(x+2, y+1, 23*width*scale, 7*height, BLACK);

		// draw F
		x += 4, y += 2;
		draw_rect(x, y, 3*width*scale, height, color);
		draw_rect(x, y, width*scale, 5*height, color);
		draw_rect(x, y+2, 3*width*scale, height, color);

		// draw hyphen (-)
		x += scale*4*width;
		draw_rect(x, y+2, 2*width*scale, height, color);

		// draw W
		x += 3*width*scale;
		draw_rect(x, y, width*scale, 4*height, color);
		draw_rect(x, y+4, 5*width*scale, height, color);
		draw_rect(x+2*width*scale, y+3, width*scale, 2*height, color);
		draw_rect(x+4*width*scale, y, width*scale, 4*height, color);

		// draw A
		x += 6*width*scale;
		draw_rect(x, y, width*scale, height*5, color);
		draw_rect(x, y, 3*width*scale, height, color);
		draw_rect(x+3*width*scale, y, width*scale, height*5, color);
		draw_rect(x, y+width*scale, 3*width*scale, height, color);

		// draw L
		x += 5*width*scale;
		draw_rect(x, y, width*scale, height*5, color);
		draw_rect(x, y+2*width*scale, 3*width*scale, height, color);

		delay(2000);
	}
}
