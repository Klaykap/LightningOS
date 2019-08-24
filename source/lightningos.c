//LIGHTNINGOS

#define MEMORY_MOUSE 0x100000

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned uint32_t;
typedef unsigned long long uint64_t;

uint32_t mouse_cursor_visible=0;
uint32_t mouse_x=0;
uint32_t mouse_y=0;

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile ( "inb %1, %0"
                   : "=a"(ret)
                   : "Nd"(port) );
    return ret;
}

static inline uint16_t inw(uint16_t port) {
    uint16_t ret;
    asm volatile ( "inw %1, %0"
                   : "=a"(ret)
                   : "Nd"(port) );
    return ret;
}

static inline uint32_t inl(uint16_t port) {
    uint32_t ret;
    asm volatile ( "inl %1, %0"
                   : "=a"(ret)
                   : "Nd"(port) );
    return ret;
}

static inline void outb(uint16_t port, uint8_t val) {
    asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

static inline void outw(uint16_t port, uint16_t val) {
    asm volatile ( "outw %0, %1" : : "a"(val), "Nd"(port) );
}

static inline void outl(uint16_t port, uint32_t val) {
    asm volatile ( "outl %0, %1" : : "a"(val), "Nd"(port) );
}

#include "drivers/vesa.c"

#include "drivers/keyboard.c"
#include "drivers/mouse.c"

#include "drivers/handlers.c"
#include "drivers/idt.c"
#include "drivers/time.c"
#include "drivers/pci.c"

#include "gui/main.c"
 
void start_lightningos(void) {
	print_line = 0;
    uint16_t *mouse_memory = (uint16_t *) 0x100000;

	mouse_cursor_visible=0;

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

	debug_line=10;
	debug_column=0;

	p("Scanning PCI...");
    pci_scan_all();

	p("Setting IDT table...");
    set_idt();
	p("Interrupts is enabled");

	p("Mouse initalizing...");
	mouse_initalize();
	mouse_cycle=0;
    clear_mouse_memory();
	mouse_x=0;
	mouse_y=0;
	mouse_cursor_visible=1;

	p("Starting GUI...");
    start_gui();
}
