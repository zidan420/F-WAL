#include "../drivers/screen.h"

void main() {
	clear_screen();
    print_at("Hello from your kernel!", 25, 10);
    print_at("Welcome to 32-bit protected mode :)", 25, 11);
}
