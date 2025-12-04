#include "../drivers/low_level.h"
#include "../drivers/screen.h"

/*
    PIC1 -> Master PIC
    PIC2 -> Slave PIC
*/

// PIC ports
#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA 0xA1

// End-of-Interrupt (EOI) command
#define PIC_EOI 0x20

// Function prototype for the assembly handler
extern void _keyboard_asm_handler();

// A struct representing a single IDT entry (8 bytes)
struct idt_entry {
    unsigned short base_low;    // The lower 16 bits of the address to jump to
    unsigned short selector;    // The GDT segment selector (e.g., 0x08 for code)
    unsigned char always0;      // This must always be zero
    unsigned char flags;        // Access flags (e.g., set for interrupts)
    unsigned short base_high;   // The upper 16 bits of the address
} __attribute__((packed));

// A struct for the IDT pointer (used by the 'lidt' assembly instruction)
struct idt_ptr {
    unsigned short limit;       // Size of the IDT - 1
    unsigned int base;          // Address of the IDT
} __attribute__((packed));

// Array to hold all 256 IDT entries
struct idt_entry idt[256];
struct idt_ptr idt_p;

// Sets an entry in the IDT
void set_idt_entry(int n, unsigned int handler) {
    idt[n].base_low = (handler & 0xFFFF);
    idt[n].selector = 0x08; // Code segment selector from GDT
    idt[n].always0 = 0;
    // P=1 (Present), DPL=00 (Ring 0), S=0 (System), Gate Type=1110 (32-bit Interrupt Gate)
    idt[n].flags = 0x8E; 
    idt[n].base_high = (handler >> 16) & 0xFFFF;
}

// Initialises the PIC, remaps IRQs, and sets up the IDT
void isr_install() {
    // 1. Remap the PIC
    // Master PIC (0x20-0x27) interrupt vectors moved from 0x08-0x0F to 0x20-0x27
    // Slave PIC (0xA0-0xA7) interrupt vectors moved from 0x70-0x77 to 0x28-0x2F
    port_byte_out(PIC1_COMMAND, 0x11); // Start initialization sequence
    port_byte_out(PIC2_COMMAND, 0x11);  // ICW1

    /*
        tells the PIC where in the IDT its IRQs start
    */
    port_byte_out(PIC1_DATA, 0x20); // ICW2: Master PIC vector offset (0x20)
    port_byte_out(PIC2_DATA, 0x28); // ICW2: Slave PIC vector offset (0x28)

    port_byte_out(PIC1_DATA, 0x04); // ICW3: Master PIC tells slave it's at IRQ2
    port_byte_out(PIC2_DATA, 0x02); // ICW3: Slave PIC tells master it's slave ID is 2

    /*
        Optional: set special modes
        BIT     Meaning
    ---------  --------------------
        0   8086/88 mode (1) vs MCS-80/85 mode (0)
        1   Auto EOI (1 = automatically send EOI after ISR)
        2   Buffered mode (master/slave)
        3   Buffered mode
        4   Special fully nested mode (for slave)
    */
    port_byte_out(PIC1_DATA, 0x01); // ICW4: 8086/AEO mode
    port_byte_out(PIC2_DATA, 0x01);

    // 2. Enable interrupts
    // Set IRQ 1 (Keyboard) in the Master PIC mask to ENABLE it (0xFD = 1111 1101)
    port_byte_out(PIC1_DATA, 0xFD);
    // Mask all interrupts on Slave PIC (we aren't using them yet)
    port_byte_out(PIC2_DATA, 0xFF);

    // 3. Setup IDT pointer
    idt_p.limit = (sizeof(struct idt_entry) * 256) - 1;
    idt_p.base = (unsigned int)&idt;
    
    // 4. Set the keyboard interrupt entry (IRQ 1 is 0x21 after remapping)
    set_idt_entry(0x21, (unsigned int)_keyboard_asm_handler);
    
    // 5. Load the IDT (Assembly instruction)
    __asm__ __volatile__("lidt (%0)" : : "r" (&idt_p));

    // 6. Enable system interrupts
    __asm__ __volatile__("sti");
}

// Function to send EOI (End of Interrupt) to the PIC
void pic_eoi(unsigned char irq) {
    /*
        Send EOI to slave PIC
    */
    if (irq >= 8) {
        port_byte_out(PIC2_COMMAND, PIC_EOI);
    }
    /*
        Send EOI to master PIC
    */
    port_byte_out(PIC1_COMMAND, PIC_EOI);
}
