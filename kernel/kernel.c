/* kernel/kernel.c */
#include "../drivers/screen.h"
#include "../drivers/timer.h"
#include "logo.h"
#include "terminal.h"
#include "filesystem.h"

void main() {
    clear_screen();
    // draw the logo at the center
    draw_logo(15, 8);
    delay(5000);
    clear_screen();
    
    // 2. Now, draw the terminal. Keyboard input should start printing 
    // scan codes here because interrupts are enabled!
    draw_terminal(); 
}