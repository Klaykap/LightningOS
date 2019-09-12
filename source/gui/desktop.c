//LIGHTNINGOS

#define DESKTOP_AQUA 0x0E78
#define DESKTOP_GREY_LIGHT 0xEF7D
#define DESKTOP_GREY_DARK 0x4208
#define DESKTOP_GREY_ACTIVE_PROGRAM 0x7BCF

#define DOWN_LINE_LINE 580

#define FULL_PROGRAMS 5

struct down_line_programs_t {
	uint32_t exist;
	uint32_t active;
	uint32_t program;
	uint8_t name[13];
} down_line_programs[5];

void start_hw_info(void);
void start_pokus(void);

void call_draw_program(uint32_t number) {
	if(number==HW_INFO) {
		start_hw_info();
		return;
	}

	if(number==POKUS) {
		start_pokus();
		return;
	}
}

void draw_time(void) {
    get_time();  //get actual time

	hide_mouse();

	draw_square(580, 728, 40, 10, DESKTOP_GREY_LIGHT);

	if(hours<10) {
    	print_var(0, DOWN_LINE_LINE, 728, BLACK);
    	print_var(hours, DOWN_LINE_LINE, 736, BLACK);
	}
	else {
    	print_var(hours, DOWN_LINE_LINE, 728, BLACK);
	}
    print(":", DOWN_LINE_LINE, 744, BLACK);
	if(minutes<10) {
    	print_var(0, DOWN_LINE_LINE, 752, BLACK);
    	print_var(minutes, DOWN_LINE_LINE, 760, BLACK);
	}
	else {
    	print_var(minutes, DOWN_LINE_LINE, 752, BLACK);
	}

	show_mouse();
}

void draw_down_line(void) {
	hide_mouse();

    //down line
    draw_square(570, 0, 800, 30, DESKTOP_GREY_LIGHT);
    draw_full_line(570, DESKTOP_GREY_DARK);

    //menu
    print("Menu", DOWN_LINE_LINE, 32, BLACK);

    //time
    draw_time();

	//programs
	for(int i=0; i<5; i++) {
		if(down_line_programs[i].exist==1) {
			//background
			if(down_line_programs[i].active==1) {
				draw_square(570, (70 + (i * 120)), 120, 29, DESKTOP_GREY_ACTIVE_PROGRAM);
			}
			else {
				draw_square(570, (70 + (i * 120)), 120, 29, DESKTOP_GREY_LIGHT);
			}
			draw_empty_square(570, (70 + (i * 120)), 120, 29, BLACK);

			//name
			print(down_line_programs[i].name, DOWN_LINE_LINE, (78 + (i * 120)), BLACK);
		}
	}

	show_mouse();
}

void draw_desktop(void) {
    //background
    draw_square(0, 0, 800, 570, DESKTOP_AQUA);
	scan_monitor(mouse_y, mouse_x, 16, 16, MOUSE_MEMORY);

    //down line
    draw_down_line();
}

void draw_menu(void) {
	mouse_cursor_visible=0;
	hide_mouse();

	//background
	scan_monitor(370, 0, 151, 200, MENU_MEMORY);
	draw_square(370, 0, 150, 200, DESKTOP_GREY_LIGHT);
	draw_empty_square(370, 0, 150, 200, BLACK);

	show_mouse();
}

void hide_menu(void) {
	hide_mouse();

	draw_monitor(370, 0, 151, 200, MENU_MEMORY);

	show_mouse();
}

void init_program_list(void) {
	for(int i=0; i<5; i++) {
		down_line_programs[i].exist=0;
		down_line_programs[i].active=0;
		down_line_programs[i].program=0;
		for(int j=0; j<13; j++) {
			down_line_programs[i].name[j]=0;
		}
	}
}

uint32_t get_window_id(void) {
	for(int i=0; i<5; i++) {
		if(down_line_programs[i].exist==0) {
			return i;
		}
	}

	return FULL_PROGRAMS;
}

void clear_active_window(void) {
	for(int i=0; i<5; i++) {
		down_line_programs[i].active=0;
	}
}

void delete_window(uint32_t number) {
	down_line_programs[number].exist=0;
	down_line_programs[number].active=0;
	down_line_programs[number].program=0;
	for(int i=0; i<13; i++) {
		down_line_programs[number].name[i]=0;
	}

	//erase free window entries
	for(int i=0; i<5; i++) {
		if(down_line_programs[i].exist==0) {  //copy next entry to it
			down_line_programs[i].exist=down_line_programs[i+1].exist;
			down_line_programs[i].active=down_line_programs[i+1].active;
			down_line_programs[i].program=down_line_programs[i+1].program;
			for(int j=0; j<13; j++) {
				down_line_programs[i].name[j]=down_line_programs[i+1].name[j];
			}

			//delete next entry
			down_line_programs[i+1].exist=0;
			down_line_programs[i+1].active=0;
			down_line_programs[i+1].program=0;
			for(int j=0; j<13; j++) {
				down_line_programs[i+1].name[j]=0;
			}
		}
	}
}
