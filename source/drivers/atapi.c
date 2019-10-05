//LIGHTNINGOS

#define ATAPI_PIO_MODE 0
#define ATAPI_SECTOR_LENGHT 2048
#define ATAPI_PACKET_COMMAND 0xA0
#define ATAPI_READ_COMMAND 0xA8

volatile uint32_t ide_secondary_interrupt=0;

int atapi_read(uint32_t base, uint32_t sector, uint32_t memory_pointer) {
	uint16_t *memory = (uint16_t *) memory_pointer;
    uint16_t size=0;
    uint32_t cycle=0;

    //Use PIO
    outb(base+1, ATAPI_PIO_MODE);

    //Maximal byte count
    outb(base+4, (ATAPI_SECTOR_LENGHT & 0xff));
    outb(base+5, (ATAPI_SECTOR_LENGHT >> 8));

    //ATAPI packet command
    outb(base+7, ATAPI_PACKET_COMMAND);

    //Wait for BSY clear and DRQ set
    cycle=0;
    for(int i=0; i<100; i++) {
        if( (inb(base+7) & 0x88)==0x08 ) {
            cycle=1;
            break;    
        }     
    }
    if(cycle==0) {  //Something is wrong
        return 0;
    }

    //Prepare value for interrupt
    ide_secondary_interrupt=0;  

    //Send ATAPI packet
    outw(base+0, ATAPI_READ_COMMAND);
    outw(base+0, (uint16_t)(sector >> 16));
    outw(base+0, (uint16_t)sector);
    outw(base+0, 0);
    outw(base+0, 1);
    outw(base+0, 0);

    //Wait for irq
    cycle=0;
    for(int i=0; i<1000; i++) {
        inb(base+7);  //wait
        if( ide_secondary_interrupt==1 ) {
            cycle=1;
            break;    
        }    
    }
    if(cycle==0) {  //Something is wrong
        return 0;
    }

    //Read lenght of input/output
    size = ( ( ((unsigned short)(inb(base+5) << 8)) | ((unsigned short)inb(base+4)) ) / 2);

    //Read data
    for(int i=0; i<size; i++) {
        memory[i]=inw(base+0);
    }

    //Reading is correctly
    return 1;

}
