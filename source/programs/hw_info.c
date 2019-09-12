//LIGHTNINGOS

#define HW_INFO_BASE_POSITION_OF_ELEMENTS_LINE 71
#define HW_INFO_BASE_POSITION_OF_ELEMENTS_COLUMN 101

uint32_t hw_info_window_id=0;

void start_hw_info(void) {
	draw_window(50, 100, 600, 400, "Hardware informations");

	//Graphic mode: 800x600x16
	draw_label( (HW_INFO_BASE_POSITION_OF_ELEMENTS_LINE + LINE(1) ), (HW_INFO_BASE_POSITION_OF_ELEMENTS_COLUMN + COLUMN(1) ), "Graphic mode:");

	draw_label( (HW_INFO_BASE_POSITION_OF_ELEMENTS_LINE + LINE(1) ), (HW_INFO_BASE_POSITION_OF_ELEMENTS_COLUMN + COLUMN(15) ), "   x   x");

	draw_label_var( (HW_INFO_BASE_POSITION_OF_ELEMENTS_LINE + LINE(1) ), (HW_INFO_BASE_POSITION_OF_ELEMENTS_COLUMN + COLUMN(15) ), vesa_x);

	draw_label_var( (HW_INFO_BASE_POSITION_OF_ELEMENTS_LINE + LINE(1) ), (HW_INFO_BASE_POSITION_OF_ELEMENTS_COLUMN + COLUMN(19) ), vesa_y);

	draw_label_var( (HW_INFO_BASE_POSITION_OF_ELEMENTS_LINE + LINE(1) ), (HW_INFO_BASE_POSITION_OF_ELEMENTS_COLUMN + COLUMN(23) ), vesa_bpp);

	//Sound card:
	draw_label( (HW_INFO_BASE_POSITION_OF_ELEMENTS_LINE + LINE(3) ), (HW_INFO_BASE_POSITION_OF_ELEMENTS_COLUMN + COLUMN(1) ), "Sound card:");

	if(hda_exist==1) {
		draw_label( (HW_INFO_BASE_POSITION_OF_ELEMENTS_LINE + LINE(3) ), (HW_INFO_BASE_POSITION_OF_ELEMENTS_COLUMN + COLUMN(13) ), "Intel HD Audio");

		draw_label( (HW_INFO_BASE_POSITION_OF_ELEMENTS_LINE + LINE(3) ), (HW_INFO_BASE_POSITION_OF_ELEMENTS_COLUMN + COLUMN(30) ), "base address:");

		draw_label_hex( (HW_INFO_BASE_POSITION_OF_ELEMENTS_LINE + LINE(3) ), (HW_INFO_BASE_POSITION_OF_ELEMENTS_COLUMN + COLUMN(44) ), hda_base);

	}
	else {
		draw_label( (HW_INFO_BASE_POSITION_OF_ELEMENTS_LINE + LINE(3) ), (HW_INFO_BASE_POSITION_OF_ELEMENTS_COLUMN + COLUMN(14) ), "Unkown/Nothing");
	}

	//ATA and ATAPI
	draw_label( (HW_INFO_BASE_POSITION_OF_ELEMENTS_LINE + LINE(5) ), (HW_INFO_BASE_POSITION_OF_ELEMENTS_COLUMN + COLUMN(1) ), "Hard disc status:");

	draw_label_hex( (HW_INFO_BASE_POSITION_OF_ELEMENTS_LINE + LINE(5) ), (HW_INFO_BASE_POSITION_OF_ELEMENTS_COLUMN + COLUMN(19) ), (inb(0x3F6)) );

	draw_label( (HW_INFO_BASE_POSITION_OF_ELEMENTS_LINE + LINE(7) ), (HW_INFO_BASE_POSITION_OF_ELEMENTS_COLUMN + COLUMN(1) ), "CDROM status:");

	draw_label_hex( (HW_INFO_BASE_POSITION_OF_ELEMENTS_LINE + LINE(7) ), (HW_INFO_BASE_POSITION_OF_ELEMENTS_COLUMN + COLUMN(15) ), (inb(0x376)) );

	//Internet card
	draw_label( (HW_INFO_BASE_POSITION_OF_ELEMENTS_LINE + LINE(9) ), (HW_INFO_BASE_POSITION_OF_ELEMENTS_COLUMN + COLUMN(1) ), "Internet card:");

	if(internet_card_exist==1 && internet_card_type==RTL8139) {
		draw_label( (HW_INFO_BASE_POSITION_OF_ELEMENTS_LINE + LINE(9) ), (HW_INFO_BASE_POSITION_OF_ELEMENTS_COLUMN + COLUMN(16) ), "RTL8139");	
	}
	else if(internet_card_exist==1) {
		draw_label( (HW_INFO_BASE_POSITION_OF_ELEMENTS_LINE + LINE(9) ), (HW_INFO_BASE_POSITION_OF_ELEMENTS_COLUMN + COLUMN(16) ), "Some network card, we havent driver for it");
	}
	else {
		draw_label( (HW_INFO_BASE_POSITION_OF_ELEMENTS_LINE + LINE(9) ), (HW_INFO_BASE_POSITION_OF_ELEMENTS_COLUMN + COLUMN(16) ), "No network card");
	}

	//UHCI	
	draw_label( (HW_INFO_BASE_POSITION_OF_ELEMENTS_LINE + LINE(11) ), (HW_INFO_BASE_POSITION_OF_ELEMENTS_COLUMN + COLUMN(1) ), "Number of UCHI ports:");

	draw_label_var( (HW_INFO_BASE_POSITION_OF_ELEMENTS_LINE + LINE(11) ), (HW_INFO_BASE_POSITION_OF_ELEMENTS_COLUMN + COLUMN(23) ), uhci_pointer);

	//EHCI	
	draw_label( (HW_INFO_BASE_POSITION_OF_ELEMENTS_LINE + LINE(13) ), (HW_INFO_BASE_POSITION_OF_ELEMENTS_COLUMN + COLUMN(1) ), "Number of ECHI ports:");

	draw_label_var( (HW_INFO_BASE_POSITION_OF_ELEMENTS_LINE + LINE(13) ), (HW_INFO_BASE_POSITION_OF_ELEMENTS_COLUMN + COLUMN(23) ), ehci_pointer);

	program = HW_INFO;

	clear_active_window();
	down_line_programs[hw_info_window_id].active=1;
	down_line_programs[hw_info_window_id].program=HW_INFO;
	down_line_programs[hw_info_window_id].name[0]='H';
	down_line_programs[hw_info_window_id].name[1]='a';
	down_line_programs[hw_info_window_id].name[2]='r';
	down_line_programs[hw_info_window_id].name[3]='d';
	down_line_programs[hw_info_window_id].name[4]='w';
	down_line_programs[hw_info_window_id].name[5]='a';
	down_line_programs[hw_info_window_id].name[6]='r';
	down_line_programs[hw_info_window_id].name[7]='e';
	down_line_programs[hw_info_window_id].name[8]=' ';
	down_line_programs[hw_info_window_id].name[9]='i';
	down_line_programs[hw_info_window_id].name[10]='n';
	down_line_programs[hw_info_window_id].name[11]='f';
	down_line_programs[hw_info_window_id].name[12]='o';
	down_line_programs[hw_info_window_id].name[13]=0;

	draw_down_line();
}

void create_hw_info(void) {
	hw_info_window_id = get_window_id();
	if(hw_info_window_id==FULL_PROGRAMS) {
		p("MOC PROGRAMOV");
		return;
	}

	down_line_programs[hw_info_window_id].exist=1;

	start_hw_info();
}


void action_click_hw_info(void) {

}
