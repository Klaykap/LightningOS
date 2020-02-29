void wait(uint32_t miliseconds);

#define BIT(x) (1 << x)

#define UHCI_COMMAND 0x00
#define UHCI_STATUS 0x02
#define UHCI_INTERRUPT 0x04
#define UHCI_FRAME_NUMBER 0x06
#define UHCI_FRAME_LIST_ADDRESS 0x08
#define UHCI_START_OF_FRAME 0x0C
#define UHCI_SC1 0x10
#define UHCI_SC2 0x12

#define USB 0

#define UHCI_PACKET_IN 0x69
#define UHCI_PACKET_OUT 0xE1
#define UHCI_PACKET_SETUP 0x2D

struct uhci_td {
	uint32_t next;
	uint32_t status;
	uint32_t header;
	uint32_t address;
	uint16_t reserved;
} uhci_mem[1024];

//uint32_t uhci_mem[50000];

uint32_t num_of_uhci_ports=0;

void write_uhci_cmd(uint32_t usb, uint16_t value) {
    outw(uhci_base[usb] + UHCI_COMMAND, value);
}

void write_uhci_sts(uint32_t usb, uint16_t value) {
    outw(uhci_base[usb] + UHCI_STATUS, value);
}

void write_uhci_int(uint32_t usb, uint16_t value) {
    outw(uhci_base[usb] + UHCI_INTERRUPT, value);
}

void write_uhci_frame_addr(uint32_t usb, uint32_t value) {
    outl(uhci_base[usb] + UHCI_FRAME_LIST_ADDRESS, value);
}

void write_uhci_frame_num(uint32_t usb, uint16_t value) {
    outw(uhci_base[usb] + UHCI_FRAME_NUMBER, value);
}

void write_uhci_sofmod(uint32_t usb, uint16_t value) {
    outb(uhci_base[usb] + UHCI_START_OF_FRAME, value);
}

void write_uhci_sc1(uint32_t usb, uint16_t offset, uint16_t value) {
    outw(uhci_base[usb] + UHCI_SC1 + offset, value);
}

void write_uhci_sc2(uint32_t usb, uint16_t offset, uint16_t value) {
    outw(uhci_base[usb] + UHCI_SC2 + offset, value);
}

uint16_t read_uhci_cmd(uint32_t usb) {
    return inw(uhci_base[usb] + UHCI_COMMAND);
}

uint16_t read_uhci_sts(uint32_t usb) {
    return inw(uhci_base[usb] + UHCI_STATUS);
}

uint16_t read_uhci_int(uint32_t usb) {
    return inw(uhci_base[usb] + UHCI_INTERRUPT);
}

uint16_t read_uhci_frame_num(uint32_t usb) {
    return inw(uhci_base[usb] + UHCI_FRAME_NUMBER);
}

uint32_t read_uhci_frame_addr(uint32_t usb) {
    return inl(uhci_base[usb] + UHCI_FRAME_LIST_ADDRESS);
}

uint16_t read_uhci_sofmod(uint32_t usb) {
    return inb(uhci_base[usb] + UHCI_START_OF_FRAME);
}

uint16_t read_uhci_sc1(uint32_t usb, uint16_t offset) {
    return inw(uhci_base[usb] + UHCI_SC1 + offset);
}

uint16_t read_uhci_sc2(uint32_t usb, uint16_t offset) {
    return inw(uhci_base[usb] + UHCI_SC2 + offset);
}

void uhci_stop(uint32_t usb) {
	write_uhci_cmd(usb, (read_uhci_cmd(usb) & 0xFFFE));
}

void uhci_run(uint32_t usb) {
	write_uhci_cmd(usb, (read_uhci_cmd(usb) | 0x0041));
}

void uhci_clear_irq(uint32_t usb) {
	write_uhci_sts(usb, (read_uhci_sts(usb) & 0xFFFE));
}

void uhci_disable_interrupts(uint32_t usb) {
	write_uhci_int(usb, 0);
}

void uhci_enable_interrupts(uint32_t usb) {
	write_uhci_int(usb, 0xF);
}

void uhci_disable_controller(uint32_t usb) {
	write_uhci_cmd(usb, 0);
}

void uhci_off_root_port(uint32_t usb, uint32_t offset) {
	write_uhci_sc1(usb, (offset * 2), 0);
}

void uhci_reset_root_port(uint32_t usb, uint32_t port) {
	write_uhci_sc1(usb, (port * 2), BIT(9));
	wait(3);
	write_uhci_sc1(usb, (port * 2), BIT(4) );
	wait(3);

	write_uhci_sc1(usb, (port * 2), 0x000E); //enable port
}

void uhci_reset(uint32_t usb) {
	uint32_t pom=0;

	p("clear UHCI memory...");
	for(int i=0; i<1024; i++) {
		uhci_mem[i].next=0;
		uhci_mem[i].status=0;
		uhci_mem[i].header=0;
		uhci_mem[i].address=0;
		uhci_mem[i].reserved=0;
	}

	p("UHCI base port");
	ph(uhci_base[usb]);
	ph((uint32_t)uhci_mem);

	uhci_stop(usb);

	pci_write(uhci_bus[usb], uhci_dev[usb], uhci_func[usb], 0x04, (pci_get_command(uhci_bus[usb], uhci_dev[usb], uhci_func[usb]) | 5) );

	p("Reseting UHCI...");
	write_uhci_cmd(usb, (read_uhci_cmd(usb) | 0x0004));
	wait(2);
	write_uhci_cmd(usb, (read_uhci_cmd(usb) | 0xFFFB));

	p("NUMBER OF UHCI PORTS");
	pom=read_uhci_sc1(usb, 0);
	for(int i=0; i<pom; i++) {
		if(read_uhci_sc1(usb, (i * 2))!=0 && read_uhci_sc1(usb, (i * 2))!=0xFFFF) {
			num_of_uhci_ports=i;
		}
	}
	pv(num_of_uhci_ports); //write num to screen

	p("Enabling Legacy USB support for switch interrupts...");
	pci_write(uhci_bus[usb], uhci_dev[usb], uhci_func[usb], 0xC0, 0x8F00);
	uhci_disable_interrupts(usb);
	uhci_disable_controller(usb);

	p("SETTING UHCI FRAME ADDRESS/LENGHT/NUMBER");
	write_uhci_sofmod(usb, 0x40);
	write_uhci_frame_addr(usb, (uint32_t)uhci_mem);
	write_uhci_frame_num(usb, 0);

	p("reset root ports");
	for(int i=0; i<num_of_uhci_ports; i++) {
		uhci_reset_root_port(usb, i);
	}

	p("enable interrupts...");
    uhci_enable_interrupts(usb);

	p("Run UHCI port...");
	write_uhci_cmd(usb, BIT(0) | BIT(4) | BIT(6) | BIT(7));
	wait(5);
	write_uhci_cmd(usb, BIT(0) | BIT(6) | BIT(7));
	wait(10);

	p("UHCI run:");
	ph(read_uhci_cmd(usb) & BIT(0));

	p("UHCI status:");
	ph(read_uhci_sts(usb) & BIT(5));
}

void uhci_receive(uint32_t usb) {
	uint32_t actual_frame=0;
	p("UHCI TD POINTER");
	actual_frame=read_uhci_frame_num(usb);
	pv(actual_frame);

	actual_frame=0;
	p("TD");
	pv(uhci_mem[actual_frame].next);
	pv(uhci_mem[actual_frame].status);
	pv(uhci_mem[actual_frame].header);
	pv(uhci_mem[actual_frame].address);
	pv(uhci_mem[actual_frame].reserved);
}

void uhci(void) {
	for(int i=0; i<uhci_pointer; i++) {
		uhci_reset(i);
	}
}

void uhci_irq(void) {
	p("UHCI IRQ");
}
