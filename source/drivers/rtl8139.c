//LIGHTNINGOS

#define ID0 0x00
#define ID4 0x04

#define BUFFER_PORT 0x30
#define COMMAND_PORT 0x37
#define CURRENT_ADDR_PORT 0x38
#define INTERRUPT_MASK_PORT 0x3C
#define INTERRUPT_STATUS_PORT 0x3E
#define TRANSMIT_CONF_PORT 0x40
#define RECEIVE_CONF_PORT 0x44

#define TRANSMIT_ADDR_PORT 0x20
#define TRANSMIT_STATUS_PORT 0x10

uint32_t transmit_port_offset=0;

void init_card_rtl8139(uint32_t buffer) {
	outb(internet_card_base + COMMAND_PORT, 0x10);
	//wait
    for(int i=0; i<10000; i++) {
        if( ( (inb(internet_card_base + COMMAND_PORT + 0x37) & 0x10) == 0) ) {
            break;    
        }    
    }

	outb(internet_card_base + COMMAND_PORT, 0x0C);

	outl(internet_card_base + TRANSMIT_CONF_PORT, 0x03000700);
	outl(internet_card_base + RECEIVE_CONF_PORT, 0x0000070A);

	outw(internet_card_base + INTERRUPT_STATUS_PORT, 0);
	outw(internet_card_base + INTERRUPT_MASK_PORT, 0xFFFF);

	outl(internet_card_base + BUFFER_PORT, buffer);

	transmit_port_offset=0;

}

void send_packet_rtl8139(uint32_t buffer, uint32_t lenght) {
	outl(internet_card_base + TRANSMIT_ADDR_PORT + transmit_port_offset, buffer);
	outl(internet_card_base + TRANSMIT_STATUS_PORT + transmit_port_offset, lenght);

	if(transmit_port_offset<12) {
		transmit_port_offset = (transmit_port_offset + 4);
	}
	else {
		transmit_port_offset = 0;
	}
}

//void get_packet_rtl8139(void)  //TODO: write this method

void irq_rtl8139(void) {
	uint32_t interrupt_status=0;

	interrupt_status = inw(internet_card_base + INTERRUPT_STATUS_PORT);

	if(interrupt_status==0x01) {  //new packet is here
		//get_packet_rtl8139();
		outw(internet_card_base + INTERRUPT_STATUS_PORT, 0x0001);
	}

	if(interrupt_status==0x04) {  //sending packet is done
		outw(internet_card_base + INTERRUPT_STATUS_PORT, 0x0004);
	}

	outw(internet_card_base + INTERRUPT_STATUS_PORT, 0x0000);
}
