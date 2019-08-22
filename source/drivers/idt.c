//LIGHTNINGOS
struct idt_entry{
	uint16_t offset_lowerbits;
	uint16_t selector;
	uint8_t zero;
	uint8_t type_attr;
	uint16_t offset_higherbits;
};

struct idt_entry idt[256];

void set_idt_entry(uint32_t entry, uint64_t method) {
	idt[entry].offset_lowerbits = (method & 0xffff);
	idt[entry].selector = 0x08;
	idt[entry].zero = 0;
	idt[entry].type_attr = 0x8e;
	idt[entry].offset_higherbits = ((method & 0xffff0000) >> 16);
}

void set_idt(void) {
    extern int load_idt();
    extern int irq0();
    extern int irq1();
    extern int irq2();
    extern int irq3();
    extern int irq4();
    extern int irq5();
    extern int irq6();
    extern int irq7();
    extern int irq8();
    extern int irq9();
    extern int irq10();
    extern int irq11();
    extern int irq12();
    extern int irq13();
    extern int irq14();
    extern int irq15();

	  unsigned long irq0_address;
    unsigned long irq1_address;
    unsigned long irq2_address;
    unsigned long irq3_address;          
    unsigned long irq4_address; 
    unsigned long irq5_address;
    unsigned long irq6_address;
    unsigned long irq7_address;
    unsigned long irq8_address;
    unsigned long irq9_address;          
    unsigned long irq10_address;
    unsigned long irq11_address;
    unsigned long irq12_address;
    unsigned long irq13_address;
    unsigned long irq14_address;          
    unsigned long irq15_address;         
	  unsigned long idt_address;
	  unsigned long idt_ptr[2];

	irq0_address = (unsigned long)irq0;
	irq1_address = (unsigned long)irq1;
	irq2_address = (unsigned long)irq2;
	irq3_address = (unsigned long)irq3;
  irq4_address = (unsigned long)irq4;
	irq5_address = (unsigned long)irq5;
	irq6_address = (unsigned long)irq6;
	irq7_address = (unsigned long)irq7;
	irq8_address = (unsigned long)irq8;
	irq9_address = (unsigned long)irq9;
	irq10_address = (unsigned long)irq10;
	irq11_address = (unsigned long)irq11;
	irq12_address = (unsigned long)irq12;
	irq13_address = (unsigned long)irq13;
	irq14_address = (unsigned long)irq14;
	irq15_address = (unsigned long)irq15;

    //remap PIC
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 40);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);
    outb(0xA1, 0x0);

    //fill idt table with values
    set_idt_entry(32, irq0_address);
    set_idt_entry(33, irq1_address);
    set_idt_entry(34, irq2_address);
    set_idt_entry(35, irq3_address);
    set_idt_entry(36, irq4_address);
    set_idt_entry(37, irq5_address);
    set_idt_entry(38, irq6_address);
    set_idt_entry(39, irq7_address);
    set_idt_entry(40, irq8_address);
    set_idt_entry(41, irq9_address);
    set_idt_entry(42, irq10_address);
    set_idt_entry(43, irq11_address);
    set_idt_entry(44, irq12_address);
    set_idt_entry(45, irq13_address);
    set_idt_entry(46, irq14_address);
    set_idt_entry(47, irq15_address);

    idt_address = (uint32_t)idt;
    idt_ptr[0] = (sizeof (struct idt_entry) * 256) + ((idt_address & 0xffff) << 16);
    idt_ptr[1] = (idt_address >> 16);

    load_idt(idt_ptr);
}
