//LIGHTNINGOS

#define FBN_START_BLOCK 2000
#define FBN_LENGHT 4000

uint32_t fbn_position=0;

void jus_read(uint64_t sector, uint16_t lenght, uint32_t memory_pointer) {
	pata(PATA_BASE, sector, lenght, READ, memory_pointer);
}

void jus_write(uint64_t sector, uint16_t lenght, uint32_t memory_pointer) {
	pata(PATA_BASE, sector, lenght, WRITE, memory_pointer);
}

void read_block(uint32_t block, uint32_t memory_pointer) {
	block = (20000 + (block * 2000));
	jus_read(block, 2000, memory_pointer);
}

void write_block(uint32_t block, uint32_t memory_pointer) {
	block = (20000 + (block * 2000));
	jus_write(block, 2000, memory_pointer);
}

void delete_block(uint32_t block) {
	block = (20000 + (block * 2000));
	jus_write(block, 2000, FREE_JUS_MEMORY);
}

void load_fbn(void) {
	jus_read(FBN_START_BLOCK, FBN_LENGHT, FBN_MEMORY);
}

void save_fbn(void) {
	jus_write(FBN_START_BLOCK, FBN_LENGHT, FBN_MEMORY);
}

void add_fbn(uint32_t fbn) {
	uint32_t *memory = (uint32_t *) FBN_MEMORY;

	for(uint32_t i=0; i<512000; i++) {
		if(memory[i]==0) {
			memory[i]=fbn;
			return;
		}
	}
}

uint32_t get_fbn(void) {
	uint32_t *memory = (uint32_t *) FBN_MEMORY;

	for(uint32_t i=0; i<512000; i++) {
		if(memory[i]>0) {
			fbn_position=i;
			return memory[i];
		}
	}

	return 0;
}

uint32_t delete_fbn(uint32_t fbn) {
	uint32_t *memory = (uint32_t *) FBN_MEMORY;

	memory[fbn]=0;
}

void read_description(uint32_t description, uint32_t memory_pointer) {
	description = (2000 + (description * 40));
	jus_read(description, 40, memory_pointer);
}

void write_description(uint32_t description, uint32_t memory_pointer) {
	description = (2000 + (description * 40));
	jus_write(description, 40, memory_pointer);
}

void delete_description(uint32_t description) {
	description = (2000 + (description * 40));
	jus_write(description, 40, FREE_JUS_MEMORY);
}

void read_file(uint32_t number, uint32_t memory_pointer) {
	uint32_t *memory = (uint32_t *) DESCRIPTION_MEMORY;
	read_description(number, DESCRIPTION_MEMORY);

	for(int i=128; i<4128; i++) {
		if(memory[i]>0) {
			read_block(memory[i], memory_pointer);
			memory_pointer += 1024000;
		}
	}
}
