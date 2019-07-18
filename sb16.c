//LIGHTNINGOS KERNEL
//Sound Blaster 16 driver

//Digital Sound Processor ports
#define DSP_RESET 0x226
#define DSP_READ 0x22A
#define DSP_WRITE 0x22C
#define DSP_BUFFER 0x22A
#define DSP_STATUS 0x22E
#define DSP_INTERRUPT 0x22F
 
//Digital Sound Processor commands
#define DSP_CMD_SAMPLE_RATE 0x41
#define DSP_CMD_TRANSFER_MODE 0xB6
#define DSP_CMD_STOP 0xD5
#define DSP_CMD_VERSION 0xE1
 
//DMA ports
#define DMA_ADDRES 0xC4
#define DMA_COUNT 0xC6
#define DMA_PAGE 0x8B
#define DMA_SINGLE_MASK 0xD4
#define DMA_TRANSFER_MODE 0xD6
#define DMA_FLIP_FLOP 0xD8

uint32_t sound_blaster_exist=0;
uint32_t sb16_version_major=0;
uint32_t sb16_version_minor=0;

uint8_t sound_buffer[0xFF];

uint8_t read_dsp(void) {
    if(inb(DSP_STATUS)==0x80) {
        return inb(DSP_READ);
    }
}

void write_dsp(uint8_t byte) {
    if(inb(DSP_WRITE)==0x00) {
        outb(DSP_WRITE, byte);
    }
}
 
void reset_dsp(void) {
          outb(DSP_RESET, 1);
          wait(3); //wait 3  microseconds
          outb(DSP_RESET, 0);
 
          if(read_dsp()==0xAA) { 
                    sound_blaster_exist=1; 
          }
}

void sb16_play(void) {
	//Sound buffer is on 0x2000 to 0x20FF

	//disable channel 5
	outb(DMA_SINGLE_MASK, 0x05);

	//reset flip-flop
	outb(DMA_FLIP_FLOP, 0x00);

	//address 0x2000
	outb(DMA_ADDRES, 0x00);
	outb(DMA_ADDRES, 0x20);

	//reset flip-flop
	outb(DMA_FLIP_FLOP, 0x00);

	//transfer mode
	outb(DMA_TRANSFER_MODE, 0x58);

	//count 0xFF
	outb(DMA_COUNT, 0xFF);
	outb(DMA_COUNT, 0x00);

	//page 0
	outb(DMA_PAGE, 0);

	//enable channel 5
	outb(DMA_SINGLE_MASK, 0x01);

	//set sample rate
	write_dsp(DSP_CMD_SAMPLE_RATE);
	write_dsp(0x00);
	write_dsp(0xFF);

	//turn speaker on
	write_dsp(0xD1);

	//send mode
	write_dsp(DSP_CMD_TRANSFER_MODE);

	//send lenght
	write_dsp(0xFF);
	write_dsp(0x00);
}

void sb16_stop(void) {
	write_dsp(DSP_CMD_STOP);
}
 
void sb16_init(void) {
          reset_dsp();
 
          //if DSP doesnt exist
          if(sound_blaster_exist==0) {
                    return;
          }
 
          //get DSP version
          write_dsp(DSP_CMD_VERSION);
          sb16_version_major=read_dsp();
          sb16_version_minor=read_dsp();
 
}
