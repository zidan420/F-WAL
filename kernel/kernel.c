#include "../drivers/screen.h"

void main() {
    // char *video = (char *)0xB8000; // Text mode VGA memory
    // video[0] = 'K';   // Character
    // video[1] = 0x0F;  // Attribute: white on black
    // while(1);         // Hang here
    
    clear_screen();
    print_at("Hello from your kernel!", 25, 10);
    print_at("Welcome to 32-bit protected mode :)", 25, 11);
}
