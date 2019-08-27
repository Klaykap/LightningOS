//LIGHTNINGOS

#define PC_SPEAKER_PORT 0x61

void turn_pc_speaker_on(void) {
	outb(PC_SPEAKER_PORT, (inb(PC_SPEAKER_PORT) | 3) );
}

void turn_pc_speaker_off(void) {
	outb(PC_SPEAKER_PORT, (inb(PC_SPEAKER_PORT) & 0xFC) );
}

void set_pc_speaker_frequence(uint16_t frequence) {
 	outb(0x43, 0xB6);
 	outb(0x42, (uint8_t)frequence);
 	outb(0x42, (uint8_t)(frequence >> 8));
}

void play_pc_speaker(uint16_t frequence) {
	set_pc_speaker_frequence(frequence);
	turn_pc_speaker_on();
}
