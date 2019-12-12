//LIGHTNINGOS

#define MOUSE_MEMORY 0x00100000
#define MENU_MEMORY 0x00100200
#define FREE_JUS_MEMORY 0x00200000
#define FBN_MEMORY 0x00300000
#define DESCRIPTION_MEMORY 0x00600000
#define UHCI_MEMORY 0x00700000
#define CORB_MEMORY 0x00800000
#define RIRB_MEMORY 0x00900000

#define STARTING 1
#define DESKTOP 2
#define MENU 3
#define HW_INFO 4

#define CLOSE_PROGRAM 100

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned uint32_t;
typedef unsigned long long uint64_t;

uint32_t mouse_cursor_visible=0;
uint32_t mouse_x=0;
uint32_t mouse_y=0;

uint32_t program=0;
uint32_t background_program=0;
uint32_t hw_info_window_id=0;

uint32_t ata_base=0;

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

void wait(void);

#include "drivers/vesa.c"

#include "drivers/pci.c"

#include "drivers/keyboard.c"
#include "drivers/mouse.c"
#include "drivers/ata.c"
#include "drivers/atapi.c"

#include "drivers/jus.c"

#include "drivers/usb_uhci.c"

#include "drivers/handlers.c"
#include "drivers/idt.c"
#include "drivers/time.c"

#include "drivers/intel_hd_audio.c"
#include "drivers/pc_speaker.c"

#include "drivers/rtl8139.c"

#include "gui/elements.c"
#include "gui/desktop.c"
#include "programs/hw_info.c"
#include "gui/main.c"
 
void start_lightningos(void) {
	program = STARTING;

	print_line = 0;

	mouse_cursor_visible=2;  //special value for starting

    read_vesa_info();
    init_vesa_font();
    clear_screen();
	p("Graphic initalized: VESA Video Mode");
    print_var(vesa_x, 0, 288, BLACK);
    print("x", 0, 312, BLACK);
    print_var(vesa_y, 0, 320, BLACK);
    print("x", 0, 344, BLACK);
    print_var(vesa_bpp, 0, 352, BLACK);
    p("Linear frame buffer:");
    print_hex(vesa_lfb, 10, 160, BLACK);

	p("Setting debug values...");
	debug_line=10;
	debug_column=0;

	p("Scanning PCI...");
	uhci_pointer=0;
	ehci_pointer=0;
    pci_scan_all();

	p("Setting IDT table...");
    set_idt();
	p("Interrupts is enabled");

	p("Choosing ATA drive...");
	outb(0x1F6, 0x40);
	if(inb(0x3F6) & 0x01) {
		outb(0x1F6, 0x40);
	}

	p("Choosing ATAPI drive...");
	outb(0x176, 0xE0);
	if(inb(0x376) & 0x01) {
		outb(0x176, 0xE0);
	}

	p("Loading JUS fbn informations...");
	//load_fbn();

	p("Initalizing mouse...");
	mouse_initalize();

	p("Starting GUI...");
	init_program_list();
	draw_desktop();
	create_hw_info();

	//Show mouse and initalize variabiles
	mouse_cycle=0;
	mouse_x=0;
	mouse_y=0;
	clear_mouse_memory();
	mouse_cursor_visible=1;
}
