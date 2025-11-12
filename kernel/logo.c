#include "../drivers/graphics.h"
#include "../drivers/screen.h"

void draw_logo(int x, int y){
	int scale = 2;
	int width = 1;
	int height = 1;

	// draw rectangle
	draw_rect(x, y, 25*width*scale, 9*height, WHITE);
	draw_rect(x+2, y+1, 23*width*scale, 7*height, BLACK);

	// draw F
	x += 4, y += 2;
	draw_rect(x, y, 3*width*scale, height, WHITE);
	draw_rect(x, y, width*scale, 5*height, WHITE);
	draw_rect(x, y+2, 3*width*scale, height, WHITE);

	// draw hyphen (-)
	x += scale*4*width;
	draw_rect(x, y+2, 2*width*scale, height, WHITE);

	// draw W
	x += 3*width*scale;
	draw_rect(x, y, width*scale, 4*height, WHITE);
	draw_rect(x, y+4, 5*width*scale, height, WHITE);
	draw_rect(x+2*width*scale, y+3, width*scale, 2*height, WHITE);
	draw_rect(x+4*width*scale, y, width*scale, 4*height, WHITE);

	// draw A
	x += 6*width*scale;
	draw_rect(x, y, width*scale, height*5, WHITE);
	draw_rect(x, y, 3*width*scale, height, WHITE);
	draw_rect(x+3*width*scale, y, width*scale, height*5, WHITE);
	draw_rect(x, y+width*scale, 3*width*scale, height, WHITE);

	// draw L
	x += 5*width*scale;
	draw_rect(x, y, width*scale, height*5, WHITE);
	draw_rect(x, y+2*width*scale, 3*width*scale, height, WHITE);
}