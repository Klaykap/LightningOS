//LIGHTNINGOS

#define LIGHT_GRAY 0xEF7D
#define DARK_GRAY 0x7BCF

#define LINE(x) (x * 10)
#define COLUMN(x) (x * 8)

void draw_window(uint32_t line, uint32_t column, uint32_t lenght, uint32_t height, uint8_t *name) {
	//background of window
	draw_square(line, column, lenght, (height + 20), LIGHT_GRAY);
	draw_square(line, column, lenght, 20, DARK_GRAY);
	draw_empty_square(line, column, lenght, (height + 20), BLACK);
	draw_line((line + 20), column, lenght, BLACK);

	//name
	print(name, (line + 5), (column + 5), BLACK);

	//X
	draw_empty_square((line + 4), (column + 583), 11, 11, BLACK);
	print("X", (line + 5), (column + 585), BLACK);

	//_
	draw_empty_square((line + 4), (column + 570), 11, 11, BLACK);
	print("_", (line + 4), (column + 572), BLACK);
}

void draw_label(uint32_t line, uint32_t column, uint8_t *message) {
	int chars=0;
	while(message[chars]!=0) {
		chars++;
		if(chars>100) {
			break;
		}
	}

	draw_square(line, column, (chars * 8), 10, LIGHT_GRAY);

	print(message, line, column, BLACK);
} 

void draw_label_hex(uint32_t line, uint32_t column, uint32_t value) {
	draw_square(line, column, (10 * 8), 10, LIGHT_GRAY); //chars is 10 - 0x????????

	print_hex(value, line, column, BLACK);
} 

void draw_label_var(uint32_t line, uint32_t column, uint32_t value) {
	int chars=0;
	for(int i=0; i<50; i++) {
		if(value < (i * 10)) {
			chars = i;
			break;
		}
	}

	draw_square(line, column, (chars * 8), 10, LIGHT_GRAY);

	print_var(value, line, column, BLACK);
} 

void draw_header_choice(uint32_t line, uint32_t column, uint32_t chars, uint32_t selected_item, uint32_t memory_pointer) {
	uint8_t *memory = (uint8_t *) memory_pointer;

	draw_square(line, column, (14 + (chars * 8)), 14, WHITE);
	draw_empty_square(line, column, (14 + (chars * 8)), 14, BLACK);

	memory_pointer = (memory_pointer + (chars * selected_item));
	
	print( (uint8_t *)memory_pointer, (line + 2), (column + 1), BLACK);

	//print arrow
	draw_line( (line + 5), (column + 1 + (chars * 8)), 10, BLACK);
	draw_line( (line + 6), (column + 2 + (chars * 8)), 8, BLACK);
	draw_line( (line + 7), (column + 3 + (chars * 8)), 6, BLACK);
	draw_line( (line + 8), (column + 4 + (chars * 8)), 4, BLACK);
	draw_line( (line + 9), (column + 5 + (chars * 8)), 2, BLACK);
}

void draw_choice(uint32_t line, uint32_t column, uint32_t chars, uint32_t items, uint32_t selected_item, uint32_t memory_pointer) {
	draw_square( (line + 14), column, (14 + (chars * 8)), (items * 12), WHITE);
	draw_empty_square( (line + 14), column, (14 + (chars * 8)), (items * 12), BLACK);

	for(int i=0; i<items; i++) {
		print( (uint8_t *)memory_pointer, (line + 15 + (i * 12)), (column + 1), BLACK);
		memory_pointer = (memory_pointer + chars);
	}
}

void draw_list(uint32_t line, uint32_t column, uint32_t chars, uint32_t items, uint32_t selected_item, uint32_t memory_pointer) {
	draw_square(line, column, (4 + (chars * 8)), (items * 12), WHITE);

	//selected item
	draw_square( (line + (selected_item * 12)), column, (4 + (chars * 8)), 12, DARK_GRAY);

	draw_empty_square(line, column, (4 + (chars * 8)), (items * 12), BLACK);

	for(int i=0; i<items; i++) {
		print( (uint8_t *)memory_pointer, (line + 15 + (i * 12)), (column + 1), BLACK);
		memory_pointer = (memory_pointer + chars);
	}
}

void draw_input(uint32_t line, uint32_t column, uint32_t chars, uint32_t cursor, uint32_t memory) {
	uint8_t *message = (uint8_t *) memory;
	uint8_t s[1];
	uint8_t mem=0;

	draw_square(line, column, (4 + (chars * 9)), 12, WHITE);
	draw_empty_square(line, column, (4 + (chars * 9)), 12, BLACK);

	s[0]=0;
	s[1]=0;

	for(int i=0; i<chars; i++) {
		mem=message[i];
		s[0]=mem;
		s[1]=0;
		print(s, (line + 2), (column + 2 + (i * 9)), BLACK);
	}

	//cursor
	draw_column( (line + 2), (column + 1 + (cursor * 9)), 9, BLACK);
}

void draw_text_area(uint32_t line, uint32_t column, uint32_t chars_lenght, uint32_t chars_height, uint32_t cursor_lenght, uint32_t cursor_height, uint32_t memory) {
	uint8_t *message = (uint8_t *) memory;
	uint8_t s[1];
	uint8_t mem=0;

	draw_square(line, column, (4 + (chars_lenght * 9)), (4 + (chars_height * 10)), WHITE);
	draw_empty_square(line, column, (4 + (chars_lenght * 9)), (4 + (chars_height * 10)), BLACK);

	s[0]=0;
	s[1]=0;

	for(int j=0; j<chars_height; j++) {
	for(int i=0; i<chars_lenght; i++) {
		mem=message[i];
		s[0]=mem;
		s[1]=0;
		print(s, (line + 2 + (j * 10)), (column + 2 + (i * 9)), BLACK);
	}
	}

	//cursor
	draw_column( (line + 2 + (cursor_height * 10)), (column + 1 + (cursor_lenght * 9)), 11, BLACK);
}

void create_button(uint8_t *message, uint32_t y, uint32_t x) {
	int chars=0;
	while(message[chars]!=0) {
		chars++;
		if(chars>100) {
			break;
		}
	}

	draw_square(y, x, ((chars*8) + 10), 14, 0xB5B6);
	draw_square((y + 5), x, ((chars*8) + 10), 11, 0xA534);
	draw_square((y + 11), x, ((chars*8) + 10), 3, 0x9CD3);
	draw_empty_square(y, x, ((chars*8) + 10), 14, BLACK);

	print(message, (y + 2), (x + 5), BLACK);
}

void draw_uncheck_checkbox(uint8_t *message, uint32_t y, uint32_t x) {
	int chars=0;
	while(message[chars]!=0) {
		chars++;
		if(chars>100) {
			break;
		}
	}

	draw_square(y, x, 10, 10, WHITE);
	draw_empty_square(y, x, 10, 10, BLACK);

	print(message, y, (x + 12), BLACK);
}

void draw_check_checkbox(uint8_t *message, uint32_t y, uint32_t x) {
	int chars=0;
	while(message[chars]!=0) {
		chars++;
		if(chars>100) {
			break;
		}
	}

	draw_square(y, x, 10, 10, WHITE);
	draw_square((y + 2), (x + 2), 7, 7, BLACK);
	draw_empty_square(y, x, 10, 10, BLACK);

	print(message, y, (x + 12), BLACK);
}

void draw_uncheck_radiobutton(uint8_t *message, uint32_t y, uint32_t x) {
	int chars=0;
	while(message[chars]!=0) {
		chars++;
		if(chars>100) {
			break;
		}
	}

	draw_square((y + 1), (x + 3), 4, 8, WHITE);
	draw_column((y + 2), (x + 2), 6, WHITE);
	draw_column((y + 2), (x + 7), 6, WHITE);
	draw_column((y + 3), (x + 1), 4, WHITE);
	draw_column((y + 3), (x + 8), 4, WHITE);

	draw_line(y, (x + 3), 4, BLACK);
	draw_line((y + 9), (x + 3), 4, BLACK);
	draw_column((y + 3), x, 4, BLACK);
	draw_column((y + 3), (x + 9), 4, BLACK);
	pixel((y + 1), (x + 2), BLACK);
	pixel((y + 1), (x + 7), BLACK);
	pixel((y + 2), (x + 1), BLACK);
	pixel((y + 2), (x + 8), BLACK);
	pixel((y + 8), (x + 2), BLACK);
	pixel((y + 8), (x + 7), BLACK);
	pixel((y + 7), (x + 1), BLACK);
	pixel((y + 7), (x + 8), BLACK);

	print(message, y, (x + 12), BLACK);
}

void draw_check_radiobutton(uint8_t *message, uint32_t y, uint32_t x) {
	int chars=0;
	while(message[chars]!=0) {
		chars++;
		if(chars>100) {
			break;
		}
	}

	draw_square((y + 1), (x + 3), 4, 8, WHITE);
	draw_column((y + 2), (x + 2), 6, WHITE);
	draw_column((y + 2), (x + 7), 6, WHITE);
	draw_column((y + 3), (x + 1), 4, WHITE);
	draw_column((y + 3), (x + 8), 4, WHITE);

	draw_line(y, (x + 3), 4, BLACK);
	draw_line((y + 9), (x + 3), 4, BLACK);
	draw_column((y + 3), x, 4, BLACK);
	draw_column((y + 3), (x + 9), 4, BLACK);
	pixel((y + 1), (x + 2), BLACK);
	pixel((y + 1), (x + 7), BLACK);
	pixel((y + 2), (x + 1), BLACK);
	pixel((y + 2), (x + 8), BLACK);
	pixel((y + 8), (x + 2), BLACK);
	pixel((y + 8), (x + 7), BLACK);
	pixel((y + 7), (x + 1), BLACK);
	pixel((y + 7), (x + 8), BLACK);

	draw_square((y + 3), (x + 3), 4, 4, BLACK);
	draw_line((y + 2), (x + 4), 2, BLACK);
	draw_line((y + 7), (x + 4), 2, BLACK);
	draw_column((y + 4), (x + 2), 2, BLACK);
	draw_column((y + 4), (x + 7), 2, BLACK);

	print(message, y, (x + 12), BLACK);
}
