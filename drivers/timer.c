/* drivers/timer.c */
void delay(unsigned int ticks) {
    // Simple busy-wait loop
    for (unsigned int i = 0; i < ticks; i++) {
        for (unsigned int j = 0; j < 100000; j++) {
            __asm__ __volatile__("nop");
        }
    }
}
