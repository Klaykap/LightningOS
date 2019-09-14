//LIGHTNINGOS

#define CORB_BASE 0x00700000
#define RIRB_BASE 0x00800000

enum hda_reg {
    REG_GCTL        = 0x08, ///< GCTL - Global Control
    REG_WAKEEN      = 0x0c, ///< WAKEEN - Wake Enable
    REG_STATESTS    = 0x0e, ///< STATESTS - State Change Status
    REG_INTCTL      = 0x20, ///< INTCTL - Interrupt Control
    REG_INTSTS      = 0x24, ///< INTSTS - Interrupt Status

    REG_CORBLBASE   = 0x40, ///< CORBLBASE - CORB Lower Base Address
    REG_CORBUBASE   = 0x44, ///< CORBUBASE - CORB Upper Base Address
    REG_CORBWP      = 0x48, ///< CORBWP - CORB Write Pointer
    REG_CORBRP      = 0x4a, ///< CORBRP - CORB Read Pointer
    REG_CORBCTL     = 0x4c, ///< CORBCTL - CORB Control
    REG_CORBSIZE    = 0x4e, ///< CORBSIZE - CORB size

    REG_RIRBLBASE   = 0x50, ///< RIRBLBASE - RIRB Lower Base Address
    REG_RIRBUBASE   = 0x54, ///< RIRBUBASE - RIRB Upper Base Address
    REG_RIRBWP      = 0x58, ///< RIRBWP - RIRB Write Pointer
    REG_RINTCNT     = 0x5a, ///< RINTCNT - Respone Interrupt Count
    REG_RIRBCTL     = 0x5c, ///< RIRBCTL - RIRB Control
    REG_RIRBSTS     = 0x5d, ///< RIRBSTS - RIRB Interrupt Status
    REG_RIRBSIZE    = 0x5e, ///< RIRBSIZE - RIRB size

    REG_DPLBASE     = 0x70, ///< DPLBASE - DMA Position Lower Base Address
    REG_DPUBASE     = 0x74, ///< DPUBASE - DMA Posiition Upper Base Address

    REG_O0_CTLL     = 0x100,    ///< Output 0 - Control Lower
    REG_O0_CTLU     = 0x102,    ///< Output 0 - Control Upper
    REG_O0_STS      = 0x103,    ///< Output 0 - Status
    REG_O0_CBL      = 0x108,    ///< Output 0 - Cyclic Buffer Length
    REG_O0_STLVI    = 0x10c,    ///< Output 0 - Last Valid Index
    REG_O0_FMT      = 0x112,    ///< Output 0 - Format
    REG_O0_BDLPL    = 0x118,    ///< Output 0 - BDL Pointer Lower
    REG_O0_BDLPU    = 0x11c,    ///< Output 0 - BDL Pointer Upper
};


void reg_outb(uint32_t offset, uint8_t value) {
	uint8_t *mem = (uint8_t *) (hda_base+offset);
	mem[0]=value;
}

void reg_outw(uint32_t offset, uint16_t value) {
	uint16_t *mem = (uint16_t *) (hda_base+offset);
	mem[0]=value;
}

void reg_outl(uint32_t offset, uint32_t value) {
	uint32_t *mem = (uint32_t *) (hda_base+offset);
	mem[0]=value;
}

uint8_t reg_inb(uint32_t offset) {
	uint8_t *mem = (uint8_t *) (hda_base+offset);
	return (mem[0]);
}

uint16_t reg_inw(uint32_t offset) {
	uint16_t *mem = (uint16_t *) (hda_base+offset);
	return (mem[0]);
}

uint32_t reg_inl(uint32_t offset) {
	uint32_t *mem = (uint32_t *) (hda_base+offset);
	return (mem[0]);
}

void corb_write(uint32_t value) {
	uint32_t *corb = (uint32_t *) CORB_BASE;

    uint16_t offset = (reg_inw(REG_CORBWP) & 0xff);
    uint16_t ring_pointer=0;
    uint16_t entry=0;

    entry = (offset + 1);
    for(int i=0; i<100; i++) {
        ring_pointer = (reg_inw(REG_CORBRP) & 0xff);
		if(ring_pointer!=entry) {
			break;
		}
    }

    corb[entry] = value;
    reg_outw(REG_CORBWP, entry);

}

uint16_t rirb_read(void) {
	uint32_t *rirb = (uint32_t *) RIRB_BASE;

    uint16_t offset = (reg_inw(REG_RIRBWP) & 0xff);
    uint16_t ring_pointer=0;

    for(int i=0; i<100; i++) {
        ring_pointer = (reg_inw(REG_RIRBWP) & 0xff);
		if(ring_pointer!=offset) {
			break;
		}
    }

    reg_outw(REG_RIRBSTS, 0x5);

	return rirb[offset+1];
}

void hda_irq(void) {
    uint32_t isr = reg_inl(REG_INTSTS);
    uint8_t sts = reg_inb(REG_O0_STS);

    reg_outl(REG_INTSTS, isr);
    reg_outb(REG_O0_STS, sts);

}

void hda_init(void) {
	//reset
    reg_outl(REG_CORBCTL, 0);
    reg_outl(REG_RIRBCTL, 0);

	wait();

    reg_outl(REG_GCTL, 0);

	wait();

    reg_outl(REG_GCTL, 1);

	wait();

	//enable interrupts
    reg_outw(REG_WAKEEN, 0xffff);
    reg_outl(REG_INTCTL, 0x800000ff);

	//CORB
	pv( reg_inb(REG_CORBSIZE));

    reg_outb(REG_CORBSIZE, 0x03);

    reg_outl(REG_CORBLBASE, CORB_BASE);
    reg_outl(REG_CORBUBASE, 0);

    reg_outb(REG_CORBCTL, 0x02);

	//RIRB
    reg_outb(REG_RIRBSIZE, 0x3);

    reg_outl(REG_RIRBLBASE, RIRB_BASE);
    reg_outl(REG_RIRBUBASE, 0);

    reg_outb(REG_RINTCNT, 0x42);

    reg_outb(REG_RIRBCTL, 0x02);

	pv( reg_inb(REG_RIRBSIZE));

	wait();
	wait();
	wait();
}

void hda(void) {
	hda_init();

	corb_write(0xF00);
	pv(rirb_read());
	corb_write(0x1);
	pv(rirb_read());
}
