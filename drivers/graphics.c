#include "screen.h"

void draw_rect(int x, int y, int width, int height, char color){
	for (int j = 0; j < height; j++){
		for (int i = 0; i < width; i++) print_char(' ', x+i, y+j, COLOR(WHITE, color));
	}
}
