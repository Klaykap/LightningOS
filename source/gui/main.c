//LIGHTNINGOS

void start_gui(void) {
	init_program_list();
	draw_desktop();
	program = DESKTOP;
	create_hw_info();
}

void key_is_press(uint8_t keycode, uint8_t ascii) {

}

void mouse_click(uint8_t button) {
	if(button==MOUSE_LEFT) {

	if(program==MENU) {
		if(get_mouse_zone(370, 0, 150, 200)==0) {
			hide_menu();
			program=background_program;
		}
	}

	if(program!=STARTING) {
		if(program!=MENU && get_mouse_zone(570, 0, 64, 30)==1) {
			background_program=program;
			draw_menu();
			program=MENU;
		}

		if(down_line_programs[0].exist==1 && get_mouse_zone(570, 70, 120, 30)==1) {
			call_draw_program(down_line_programs[0].program);
		}

		if(down_line_programs[1].exist==1 && get_mouse_zone(570, 190, 120, 30)==1) {
			call_draw_program(down_line_programs[1].program);
		}

		if(down_line_programs[2].exist==1 && get_mouse_zone(570, 260, 120, 30)==1) {
			call_draw_program(down_line_programs[2].program);
		}

		if(down_line_programs[3].exist==1 && get_mouse_zone(570, 330, 120, 30)==1) {
			call_draw_program(down_line_programs[3].program);
		}

		if(down_line_programs[4].exist==1 && get_mouse_zone(570, 400, 120, 30)==1) {
			call_draw_program(down_line_programs[4].program);
		}
	}

	if(program==HW_INFO) {
		action_click_hw_info();
	}

	}
}

void mouse_scroll(uint8_t scroll) {

}

void ata_error(void) {
	p("DISK ERROR!");
}
