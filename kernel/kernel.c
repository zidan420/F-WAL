#include "../drivers/screen.h"
#include "../drivers/timer.h"
#include "logo.h"
#include "terminal.h"

void main() {
    clear_screen();
    // draw the logo at the center
    draw_logo(15, 8);

    delay(1000);

    clear_screen();
    // draw the logo at the center
    draw_terminal();
}
