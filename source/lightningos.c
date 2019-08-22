//LIGHTNINGOS

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned uint32_t;
typedef unsigned long long uint64_t;

static inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    asm volatile ( "inb %1, %0"
                   : "=a"(ret)
                   : "Nd"(port) );
    return ret;
}

static inline uint16_t inw(uint16_t port)
{
    uint16_t ret;
    asm volatile ( "inw %1, %0"
                   : "=a"(ret)
                   : "Nd"(port) );
    return ret;
}

static inline uint32_t inl(uint16_t port)
{
    uint32_t ret;
    asm volatile ( "inl %1, %0"
                   : "=a"(ret)
                   : "Nd"(port) );
    return ret;
}

static inline void outb(uint16_t port, uint8_t val)
{
    asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

static inline void outw(uint16_t port, uint16_t val)
{
    asm volatile ( "outw %0, %1" : : "a"(val), "Nd"(port) );
}

static inline void outl(uint16_t port, uint32_t val)
{
    asm volatile ( "outl %0, %1" : : "a"(val), "Nd"(port) );
}

#include "drivers/vesa.c"

#include "drivers/keyboard.c"

#include "drivers/handlers.c"
#include "drivers/idt.c"
#include "drivers/time.c"
#include "drivers/pci.c"

#include "gui/main.c"
 
void start_lightningos(void) {
	print_line = 0;

    read_vesa_info();
    init_vesa_font();
    clear_screen();
	p("Graphic initalized: VESA Video Mode");
    print_var(vesa_x, 0, 36, BLACK);
    print("x", 0, 39, BLACK);
    print_var(vesa_y, 0, 40, BLACK);
    print("x", 0, 43, BLACK);
    print_var(vesa_bpp, 0, 44, BLACK);
    p("Linear frame buffer:");
    print_hex(vesa_lfb, 1, 20, BLACK);

	p("Scanning PCI...");
    pci_scan_all();

	p("Setting IDT table...");
    set_idt();
	p("Interrupts is enabled");

	p("Starting GUI...");
    start_gui();
}
