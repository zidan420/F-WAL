#ifndef KEYBOARD_H
#define KEYBOARD_H

#define KEYBOARD_DATA_PORT 0x60
/*
Reading port 0x64 gives back an 8-bit value:
Bit 7  Output buffer full (mouse)
Bit 6  Input buffer full?
Bit 5  Timeout error
Bit 4  Parity error
Bit 3  Command/data (from controller)
Bit 2  System flag
Bit 1  Input buffer full (cannot send command)
Bit 0  Output buffer full (data available)
*/
#define KEYBOARD_STATUS_PORT 0x64

/*
Each PS/2 Keyboard key produces:
1. A make code (when key is pressed)
2. A break code (when key is released)

break_code = make_code + 0x80
*/
#define LSHIFT_MAKE   0x2A
#define LSHIFT_BREAK  0xAA
#define RSHIFT_MAKE   0x36
#define RSHIFT_BREAK  0xB6

// ----- Basic QWERTY Keymap (Set 1) -----
static const unsigned char QWERTY_MAP[] = {
    0,  0, '1','2','3','4','5','6','7','8','9','0','-','=', '\b','\t',
    'q','w','e','r','t','y','u','i','o','p','[',']','\n', 0,  'a','s',
    'd','f','g','h','j','k','l',';','\'','`', 0, '\\','z','x','c','v',
    'b','n','m',',','.','/', 0, '*', 0, ' ', 0
};

// ----- Shifted Keymap -----
static const unsigned char SHIFTED_MAP[] = {
    0,  0, '!','@','#','$','%','^','&','*','(',')','_','+', '\b','\t',
    'Q','W','E','R','T','Y','U','I','O','P','{','}','\n', 0,  'A','S',
    'D','F','G','H','J','K','L',':','"','~', 0, '|','Z','X','C','V',
    'B','N','M','<','>','?', 0, 0, 0, ' ', 0
};

int keyboard_has_key();
char keyboard_read_char();

#endif
