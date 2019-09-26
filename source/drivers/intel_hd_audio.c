//LIGHTNINGOS

uint32_t corb_pointer=0;

void hda_outb(uint32_t offset, uint8_t value) {
	uint8_t *mem = (uint8_t *) (hda_base+offset);
	mem[0]=value;
}

void hda_outw(uint32_t offset, uint16_t value) {
	uint16_t *mem = (uint16_t *) (hda_base+offset);
	mem[0]=value;
}

void hda_outl(uint32_t offset, uint32_t value) {
	uint32_t *mem = (uint32_t *) (hda_base+offset);
	mem[0]=value;
}

uint8_t hda_inb(uint32_t offset) {
	uint8_t *mem = (uint8_t *) (hda_base+offset);
	return (mem[0]);
}

uint16_t hda_inw(uint32_t offset) {
	uint16_t *mem = (uint16_t *) (hda_base+offset);
	return (mem[0]);
}

uint32_t hda_inl(uint32_t offset) {
	uint32_t *mem = (uint32_t *) (hda_base+offset);
	return (mem[0]);
}

void corb_write_command(uint32_t cad, uint32_t nid, uint32_t verb, uint32_t command) {
	uint32_t *corb = (uint32_t *) CORB_MEMORY;

	corb[corb_pointer]=( (cad << 28) | (nid << 20) | (verb << 8) | (command) );

	corb_pointer++;
}

void hda(void) {
	uint32_t *corb = (uint32_t *) CORB_MEMORY;
	uint64_t *rirb = (uint64_t *) RIRB_MEMORY;

	hda_outl(0x40, CORB_MEMORY);  //CORB buffer
	hda_outl(0x50, RIRB_MEMORY);  //RIRB buffer
	hda_outl(0x44, 0x00000000);  //CORB buffer
	hda_outl(0x54, 0x00000000);  //RIRB buffer

	//write commands into CORB
	for(int i=0; i<16; i++) {
		corb_write_command(0, i, 0xF00, 4);
	}

	hda_outw(0x48, 16); //number of commands to corb

	hda_outw(0x4A, 0x8000); //reset corb

	wait();
	wait();

	while(hda_inw(0x4A)!=0x8000) {}  //wait
	hda_outw(0x4A, 0x0000); //reset corb

	hda_outb(0x5C, (hda_inb(0x5C) | 2));
	hda_outb(0x5C, (hda_inb(0x5C) & 250));
	hda_outb(0x4C, (hda_inb(0x4C) | 2));
	hda_outb(0x4C, (hda_inb(0x4C) & 0xFE));

	while(hda_inw(0x58)==0) {} //here is forever cycle

	for(int i=0; i<20; i++) {
		pv(rirb[i]);
	}
}
