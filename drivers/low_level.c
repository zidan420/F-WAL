/* drivers/low_level.c */
/*
    Listen for a byte in a specified port
*/
unsigned char port_byte_in(unsigned short port) {
    unsigned char result;
    __asm__ __volatile__ ("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}
/*
    Write a byte to a specified port
*/
void port_byte_out(unsigned short port, unsigned char data) {
    __asm__ __volatile__ ("out %%al, %%dx" : : "a" (data), "d" (port));
}
/*
    Listen for a word in a specified port
*/
unsigned short port_word_in(unsigned short port) {
    unsigned short result;
    __asm__ __volatile__ ("in %%dx, %%ax" : "=a" (result) : "d" (port));
    return result;
}
/*
    Write a word to a specified port
*/
void port_word_out(unsigned short port, unsigned short data) {
    __asm__ __volatile__ ("out %%ax, %%dx" : : "a" (data), "d" (port));
}
