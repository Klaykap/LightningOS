//LIGHTNINGOS
//This driver work for PS/2 mouse

uint32_t drag_and_drop=0;
uint8_t mouse_button=0;

#define MOUSE_NO_BUTTON 0
#define MOUSE_LEFT 1
#define MOUSE_RIGHT 2
#define MOUSE_MIDDLE 3
#define MOUSE_4TH 4
#define MOUSE_5TH 5
#define MOUSE_SCROLL_UP 1
#define MOUSE_SCROLL_DOWN 2

#define MOUSE_READ_PORT 0x60
#define MOUSE_WRITE_PORT 0x64

void mouse_click(uint8_t button);
void mouse_scroll(uint8_t scroll);

void mouse_wait(uint32_t type) {
    if(type==0) {
        for(uint32_t i=0; i<100000; i++) {
            if((inb(MOUSE_WRITE_PORT) & 1)==1) {
                return;
            }
        }
        return;
    }
    else {
        for(uint32_t i=0; i<100000; i++) {
            if((inb(MOUSE_WRITE_PORT) & 2)==0) {
                return;
            }
        }
        return;
    }
}

void mouse_write(uint8_t value) {
    mouse_wait(1);
    outb(MOUSE_WRITE_PORT, 0xD4);
    mouse_wait(1);
    outb(MOUSE_READ_PORT, value);
}

uint8_t mouse_read(void) {
    mouse_wait(0);
    return inb(MOUSE_READ_PORT);
}

void mouse_initalize(void) {
    uint8_t status=0;

    mouse_wait(1);
    outb(MOUSE_WRITE_PORT,0xA8);

    mouse_wait(1);
    outb(MOUSE_WRITE_PORT,0x20);

    mouse_wait(0);
    status = (inb(MOUSE_READ_PORT) | 2);

    mouse_wait(1);
    outb(MOUSE_WRITE_PORT, 0x60);

    mouse_wait(1);
    outb(MOUSE_READ_PORT, status);

    //set sample rate
    mouse_write(0xF6);
    mouse_read();

    //start sending packets
    mouse_write(0xF4);
    mouse_read();

}

uint32_t get_mouse_zone(uint32_t y, uint32_t x, uint32_t lenght, uint32_t height) {
	if(mouse_x>=x && mouse_y>=y && mouse_x<=(x+lenght) && mouse_y<=(y+height)) {
		return 1;
	}
	else {
		return 0;
	}
}

uint32_t get_mouse_block_line(uint32_t y, uint32_t x, uint32_t lenght) {
	if(mouse_x>x) {
		return ( (mouse_x - x) / lenght);
	}
	else {
		return 0;
	}
}

uint32_t get_mouse_block_column(uint32_t y, uint32_t x, uint32_t height) {
	if(mouse_y>y) {
		return ( (mouse_y - y) / height);
	}
	else {
		return 0;
	}
}

void handle_mouse_packet(uint8_t mouse_move_x, uint8_t mouse_move_y, uint8_t mouse_packet_button, uint8_t mouse_packet_scroll) {
	int test=0;

        //up
		if(mouse_move_y<0x80) {
			draw_monitor(mouse_y, mouse_x, 16, 16, 0x100000);
			test = (mouse_y - mouse_move_y);
			if(test < 0) {
				mouse_y = 0;
			}
			else {
				mouse_y = test;
			}
			scan_monitor(mouse_y, mouse_x, 16, 16, 0x100000);
			draw_mouse(mouse_y, mouse_x);
		}

        //down
		if(mouse_move_y>0x80) {
			mouse_move_y = (0xFF - mouse_move_y);
			draw_monitor(mouse_y, mouse_x, 16, 16, 0x100000);
			test=(mouse_y + mouse_move_y);
			if(test > vesa_y) {
				mouse_y = vesa_y;
			}
			else {
				mouse_y = test;
			}
			scan_monitor(mouse_y, mouse_x, 16, 16, 0x100000);
			draw_mouse(mouse_y, mouse_x);
		}

        //right
		if(mouse_move_x<0x80) {
			draw_monitor(mouse_y, mouse_x, 16, 16, 0x100000);
			test = (mouse_x + mouse_move_x);
			if(test > vesa_x) {
				mouse_x = vesa_x;
			}
			else {
				mouse_x = test;
			}
			scan_monitor(mouse_y, mouse_x, 16, 16, 0x100000);
			draw_mouse(mouse_y, mouse_x);
		}

        //left
		if(mouse_move_x>0x80) {
			mouse_move_x = (0xFF - mouse_move_x);
			draw_monitor(mouse_y, mouse_x, 16, 16, 0x100000);
			test=(mouse_x - mouse_move_x);
			if(test < 0) {
				mouse_x = 0;
			}
			else {
				mouse_x = test;
			}
			scan_monitor(mouse_y, mouse_x, 16, 16, 0x100000);
			draw_mouse(mouse_y, mouse_x);
		}

    if ((mouse_packet_scroll & 0x01)==0x01) {    //scroll up
        mouse_scroll(MOUSE_SCROLL_UP);
    }

    if ((mouse_packet_scroll & 0x0F)==0x0F) {    //scroll down
        mouse_scroll(MOUSE_SCROLL_DOWN);
    }

    if (mouse_packet_button & 0x4) {      //middle button
		if(drag_and_drop==0) {
        	mouse_button=MOUSE_MIDDLE;
        	mouse_click(MOUSE_MIDDLE);
			drag_and_drop=1;
		}
    }
    else if (mouse_packet_button & 0x2) {      //right button 
		if(drag_and_drop==0) {        
        	mouse_button=MOUSE_RIGHT;
        	mouse_click(MOUSE_RIGHT);
			drag_and_drop=1;
		}
    }
    else if (mouse_packet_button & 0x1) {      //left button
		if(drag_and_drop==0) {
        	mouse_button=MOUSE_LEFT;
        	mouse_click(MOUSE_LEFT);
			drag_and_drop=1;
		}
    }
    else if (mouse_packet_scroll & 0x1) {    //4th button
		if(drag_and_drop==0) {
        	mouse_button=MOUSE_4TH;
        	mouse_click(MOUSE_4TH);
			drag_and_drop=1;
		}
    }
    else if (mouse_packet_scroll & 0x2) {    //5th button
		if(drag_and_drop==0) {
        	mouse_button=MOUSE_5TH;
        	mouse_click(MOUSE_5TH);
			drag_and_drop=1;
		}
    }
    else {    //no button
        mouse_button=MOUSE_NO_BUTTON;
		drag_and_drop=0;
    }
}

void hide_mouse(void) {
	if(mouse_cursor_visible==0 || mouse_cursor_visible==2) {
		return;
	}

	mouse_cursor_visible=0;
    draw_monitor(mouse_y, mouse_x, 16, 16, MOUSE_MEMORY);
}

void show_mouse(void) {
	if(mouse_cursor_visible==1 || mouse_cursor_visible==2) {
		return;
	}

    scan_monitor(mouse_y, mouse_x, 16, 16, MOUSE_MEMORY);
	draw_mouse(mouse_y, mouse_x);
	mouse_cursor_visible=1;
}
