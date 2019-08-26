//LIGHTNINGOS

#define PATA_BASE 0x1F0

#define READ 1
#define WRITE 2

void ata_error(void);

uint32_t pata(uint32_t base, uint64_t sector, uint16_t number_of_sectors, uint32_t type, uint32_t memory_pointer) {
	uint16_t *buffer = (uint16_t *) memory_pointer;
    uint32_t cycle=0;
	uint32_t offset=0;

    //PIO48
    outb(base+2, (unsigned char)(number_of_sectors >> 8));
    outb(base+3, (unsigned char)(sector >> 24));
    outb(base+4, (unsigned char)(sector >> 32));
    outb(base+5, (unsigned char)(sector >> 40));
    outb(base+2, (unsigned char)number_of_sectors);
    outb(base+3, (unsigned char)sector);
    outb(base+4, (unsigned char)(sector >> 8));
    outb(base+5, (unsigned char)(sector >> 16));
    //type
    if(type==READ) {
    outb(base+7, 0x24);  //Send command read
    }
    else {
    outb(base+7, 0x34);  //Send command write
    }

    //wait for BSY clear and DRQ set
    cycle=0;
    for(int i=0; i<1000; i++) {
        inb(base+0x206);  //wait
        if( (inb(base+0x206) & 0x88)==0x08 ) {  //drq is set
            cycle=1;
            break;    
        }    
    }
    if(cycle==0) {  //Something is wrong
        if( (inb(base+0x206) & 0x01)==0x01 ) {  //bad block
            ata_error();   
        }
        return 0;
    }

    if( (inb(base+0x206) & 0x01)==0x01 ) {  //bad block
        ata_error();    
    }

	for(int i=0; i<number_of_sectors; i++) {
    	for (int j=0; j<256; j++) {
        	if(type==READ) {
            	buffer[offset] = inw(base + 0);
        	}
        	else {
            	outw(base + 0, buffer[offset]);
        	}
			offset++;
    	}
	}

    return 1;
}
