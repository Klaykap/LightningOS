//LIGHTNINGOS

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

void hda(void) {
	uint32_t *corb = (uint32_t *) 0x00700000;
	uint32_t *rirb = (uint32_t *) 0x00800000;

	hda_outl(0x40, 0x00700000);  //CORB buffer
	hda_outl(0x50, 0x00800000);  //RIRB buffer
	hda_outl(0x44, 0x00000000);  //CORB buffer
	hda_outl(0x54, 0x00000000);  //RIRB buffer

	//write commands into CORB
	corb[0]=0x04F00000;

	hda_outw(0x48, 1); //number of commands to corb
	hda_outw(0x4A, 0x0080); //reset corb

	wait();

	hda_outw(0x4A, 0x0000); //reset corb

	hda_outw(0x58, 1); //number of commands to rirb

	hda_outb(0x5C, (hda_inb(0x5C) | 2));
	hda_outb(0x4C, (hda_inb(0x4C) | 2));

	wait();

	pv(rirb[0]);
	pv(rirb[1]);
	pv(rirb[2]);
	pv(rirb[3]);
}
