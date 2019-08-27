//LIGHTNINGOS

#define PCI_FIRST_BYTE 1
#define PCI_SECOND_BYTE 2
#define PCI_THIRD_BYTE 3
#define PCI_FOURTH_BYTE 4

#define PCI_FIRST_WORD 5
#define PCI_SECOND_WORD 6

#define PCI_DWORD 7

#define BAR_PORT_BASE 1
#define BAR_MEMORY_BASE 0

uint16_t vendorid=0;
uint16_t deviceid=0;
uint8_t class=0;
uint8_t subclass=0;
uint8_t progif=0;
uint8_t header=0;
uint32_t bar=0;
uint32_t bar0_type=0;
uint32_t bar0_value=0;
uint32_t bar1_type=0;
uint32_t bar1_value=0;
uint32_t bar2_type=0;
uint32_t bar2_value=0;
uint32_t bar3_type=0;
uint32_t bar3_value=0;
uint32_t bar4_type=0;
uint32_t bar4_value=0;
uint32_t bar5_type=0;
uint32_t bar5_value=0;
uint8_t interrupt_line=0;
uint8_t interrupt_pin=0;

uint32_t ohci_base[10];
uint32_t ohci_pointer=0;

uint32_t uhci_base[10];
uint32_t uhci_bus[10];
uint32_t uhci_dev[10];
uint32_t uhci_func[10];
uint32_t uhci_pointer=0;

uint32_t ehci_base[10];
uint32_t ehci_pointer=0;

#define RTL8139 1
uint32_t internet_card_type=0;
uint32_t internet_card_base=0;
uint8_t internet_card_bus=0;
uint8_t internet_card_device=0;
uint8_t internet_card_function=0;
uint32_t internet_irq=0;

uint32_t pci_read(uint32_t bus, uint32_t dev, uint32_t func, uint32_t offset, uint32_t lenght) {
    uint32_t value=0;
    uint32_t address = (0x80000000 | (bus<<16) | (dev<<11) | (func<<8) | (offset&0xFC));

    outl(0x0CF8,address);
    value = inl(0x0CFC);

    if(lenght==PCI_DWORD) {
        return value;
    }

    if(lenght==PCI_FIRST_WORD) {
        return ((uint16_t)(value)); 
    }
    if(lenght==PCI_SECOND_WORD) {
        return ((uint16_t)(value >> 16));
    }

    if(lenght==PCI_FIRST_BYTE) {
        return ((uint8_t)(value));
    }
    if(lenght==PCI_SECOND_BYTE) {
        return ((uint8_t)(value >> 8));
    }
    if(lenght==PCI_THIRD_BYTE) {
        return ((uint8_t)(value >> 16));
    }
    if(lenght==PCI_FOURTH_BYTE) {
        return ((uint8_t)(value >> 24));
    }

}

void pci_write(uint32_t bus,uint32_t dev,uint32_t func,uint32_t offset,uint32_t value) {
    uint32_t address = ( 0x80000000 | (bus<<16) | (dev<<11) | (func<<8) | (offset&0xFC) );

    outl(0xCF8, address);
    outl(0xCFC, value);
}

uint32_t pci_get_vendor(uint32_t bus, uint32_t dev, uint32_t func)  {
    return pci_read(bus, dev, func, 0, PCI_FIRST_WORD);
}

uint32_t pci_get_device(uint32_t bus, uint32_t dev, uint32_t func)  {
    return pci_read(bus, dev, func, 0, PCI_SECOND_WORD);
}

uint32_t pci_get_command(uint32_t bus, uint32_t dev, uint32_t func)  {
    return pci_read(bus, dev, func, 4, PCI_FIRST_WORD);
}

uint32_t pci_get_status(uint32_t bus, uint32_t dev, uint32_t func)  {
    return pci_read(bus, dev, func, 4, PCI_SECOND_WORD);
}

uint32_t pci_get_progif(uint32_t bus, uint32_t dev, uint32_t func)  {
    return pci_read(bus, dev, func, 8, PCI_SECOND_BYTE);
}

uint32_t pci_get_subclass(uint32_t bus, uint32_t dev, uint32_t func)  {
    return pci_read(bus, dev, func, 8, PCI_THIRD_BYTE);
}

uint32_t pci_get_class(uint32_t bus, uint32_t dev, uint32_t func)  {
    return pci_read(bus, dev, func, 8, PCI_FOURTH_BYTE);
}

uint32_t pci_get_header(uint32_t bus, uint32_t dev, uint32_t func)  {
    return pci_read(bus, dev, func, 12, PCI_THIRD_BYTE);
}

uint32_t pci_get_bar0(uint32_t bus, uint32_t dev, uint32_t func)  {
    return pci_read(bus, dev, func, 0x10, PCI_DWORD);
}

uint32_t pci_get_bar1(uint32_t bus, uint32_t dev, uint32_t func)  {
    return pci_read(bus, dev, func, 0x14, PCI_DWORD);
}

uint32_t pci_get_bar2(uint32_t bus, uint32_t dev, uint32_t func)  {
    return pci_read(bus, dev, func, 0x18, PCI_DWORD);
}

uint32_t pci_get_bar3(uint32_t bus, uint32_t dev, uint32_t func)  {
    return pci_read(bus, dev, func, 0x1C, PCI_DWORD);
}

uint32_t pci_get_bar4(uint32_t bus, uint32_t dev, uint32_t func)  {
    return pci_read(bus, dev, func, 0x20, PCI_DWORD);
}

uint32_t pci_get_bar5(uint32_t bus, uint32_t dev, uint32_t func)  {
    return pci_read(bus, dev, func, 0x24, PCI_DWORD);
}

uint32_t pci_get_interrupt_line(uint32_t bus, uint32_t dev, uint32_t func)  {
    return pci_read(bus, dev, func, 0x3C, PCI_FIRST_BYTE);
}

uint32_t pci_get_interrupt_pin(uint32_t bus, uint32_t dev, uint32_t func)  {
    return pci_read(bus, dev, func, 0x3C, PCI_SECOND_BYTE);
}

void pci_scan_device(uint32_t bus, uint32_t dev, uint32_t func) {
    vendorid=pci_get_vendor(bus, dev, func);
    deviceid=pci_get_device(bus, dev, func);

    class=pci_get_class(bus, dev, func);
    subclass=pci_get_subclass(bus, dev, func);
    progif=pci_get_progif(bus, dev, func);

    bar=pci_get_bar0(bus, dev, func);
    if( (bar & 1) == 1) {  //IO base port
        bar0_type=BAR_PORT_BASE;
        bar0_value=(bar & 0xFFFFFFFC);
    }
    else {  //Memory base
        bar0_type=BAR_MEMORY_BASE;
        if( ((bar & 6) >> 1) == 0x00) {  //32 memory base
            bar0_value=(bar & 0xFFFFFFF0);
        }
        else if ( ((bar & 6) >> 1) == 0x01) {  //16 memory base
            bar0_value=(bar & 0x0000FFF0);
        }
    }

    bar=pci_get_bar1(bus, dev, func);
    if( (bar & 1) == 1) {  //IO base port
        bar1_type=BAR_PORT_BASE;
        bar1_value=(bar & 0xFFFFFFFC);
    }
    else {  //Memory base
        bar1_type=BAR_MEMORY_BASE;
        if( ((bar & 6) >> 1) == 0x00) {  //32 memory base
            bar1_value=(bar & 0xFFFFFFF0);
        }
        else if ( ((bar & 6) >> 1) == 0x01) {  //16 memory base
            bar1_value=(bar & 0x0000FFF0);
        }
    }

    bar=pci_get_bar2(bus, dev, func);
    if( (bar & 1) == 1) {  //IO base port
        bar2_type=BAR_PORT_BASE;
        bar2_value=(bar & 0xFFFFFFFC);
    }
    else {  //Memory base
        bar2_type=BAR_MEMORY_BASE;
        if( ((bar & 6) >> 1) == 0x00) {  //32 memory base
            bar2_value=(bar & 0xFFFFFFF0);
        }
        else if ( ((bar & 6) >> 1) == 0x01) {  //16 memory base
            bar2_value=(bar & 0x0000FFF0);
        }
    }

    bar=pci_get_bar3(bus, dev, func);
    if( (bar & 1) == 1) {  //IO base port
        bar3_type=BAR_PORT_BASE;
        bar3_value=(bar & 0xFFFFFFFC);
    }
    else {  //Memory base
        bar3_type=BAR_MEMORY_BASE;
        if( ((bar & 6) >> 1) == 0x00) {  //32 memory base
            bar3_value=(bar & 0xFFFFFFF0);
        }
        else if ( ((bar & 6) >> 1) == 0x01) {  //16 memory base
            bar3_value=(bar & 0x0000FFF0);
        }
    }

    bar=pci_get_bar4(bus, dev, func);
    if( (bar & 1) == 1) {  //IO base port
        bar4_type=BAR_PORT_BASE;
        bar4_value=(bar & 0xFFFFFFFC);
    }
    else {  //Memory base
        bar4_type=BAR_MEMORY_BASE;
        if( ((bar & 6) >> 1) == 0x00) {  //32 memory base
            bar4_value=(bar & 0xFFFFFFF0);
        }
        else if ( ((bar & 6) >> 1) == 0x01) {  //16 memory base
            bar4_value=(bar & 0x0000FFF0);
        }
    }

    bar=pci_get_bar5(bus, dev, func);
    if( (bar & 1) == 1) {  //IO base port
        bar5_type=BAR_PORT_BASE;
        bar5_value=(bar & 0xFFFFFFFC);
    }
    else {  //Memory base
        bar5_type=BAR_MEMORY_BASE;
        if( ((bar & 6) >> 1) == 0x00) {  //32 memory base
            bar5_value=(bar & 0xFFFFFFF0);
        }
        else if ( ((bar & 6) >> 1) == 0x01) {  //16 memory base
            bar5_value=(bar & 0x0000FFF0);
        }
    }

    interrupt_line=pci_get_interrupt_line(bus, dev, func);
    interrupt_pin=pci_get_interrupt_pin(bus, dev, func);

	//Network card
	if(class==0x02 && subclass==0x00 && progif==0x00) {
        internet_card_bus=bus;
        internet_card_device=dev;
        internet_card_function=func;
		internet_irq=interrupt_line;

		if(vendorid==0x10EC && deviceid==0x8139) {
			internet_card_type=RTL8139;
			internet_card_base=bar0_value;
		}
	}

	//Usb 1.0 - OHCI
	if(class==0x0C && subclass==0x03 && progif==0x10) {
		ohci_base[ohci_pointer]=bar4_value;
		ohci_pointer++;
	}

	//Usb 1.1 - UHCI
	if(class==0x0C && subclass==0x03 && progif==0x00) {
		uhci_base[uhci_pointer]=bar4_value;
		uhci_bus[uhci_pointer]=bus;
		uhci_dev[uhci_pointer]=dev;
		uhci_func[uhci_pointer]=func;
		uhci_pointer++;
	}

	//Usb 2.0 - EHCI
	if(class==0x0C && subclass==0x03 && progif==0x20) {
		ehci_base[ehci_pointer]=bar4_value;
		ehci_pointer++;
	}

}

void pci_scan_all(void) {

    for(int bus=0; bus<256; bus++) {
        for(int device=0; device<32; device++) {
            
            if( (pci_get_header(bus, device, 0) & 0x80) != 0) {
                for(int function=0; function<8; function++) {   //multi-function device
                    if(pci_get_vendor(bus, device, function) != 0xFFFF) {
                        pci_scan_device(bus, device, function);
                    }
                }
            }
            else if (pci_get_vendor(bus, device, 0) != 0xFFFF) {    //single-function device
                pci_scan_device(bus, device, 0);
            }

        }
    }

}
