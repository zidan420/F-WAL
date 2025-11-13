#include "../drivers/screen.h"
#include "../drivers/timer.h"
#include "logo.h"
#include "terminal.h"

// Declare the ISR installation function
void isr_install(); 

void main() {
    // 1. Set up the IDT and PIC
    isr_install(); 
    
    clear_screen();
    // draw the logo at the center
    draw_logo(15, 8);
    delay(1000);
    clear_screen();
    
    // 2. Now, draw the terminal. Keyboard input should start printing 
    // scan codes here because interrupts are enabled!
    draw_terminal(); 
}
