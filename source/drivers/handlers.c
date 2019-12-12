//LIGHTNINGOS

uint8_t mouse_packet[3];

uint32_t mouse_cycle=0;
uint32_t debug_line=0;
uint32_t debug_column=0;

uint32_t handle_irq12=0;

uint32_t wait_var=0;

uint32_t time_cycle=0;

#define MOUSE_NO_BUTTON 0
#define MOUSE_LEFT 1
#define MOUSE_RIGHT 2
#define MOUSE_MIDDLE 3
#define MOUSE_4TH 4
#define MOUSE_5TH 5
#define MOUSE_SCROLL_UP 1
#define MOUSE_SCROLL_DOWN 2

void key_is_press(uint8_t keycode, uint8_t ascii);
void draw_time(void);
void hda_irq(void);

void wait(void) {
	uint32_t tmp=0;

	tmp=wait_var;

	for(int i=0; i<100; i++) {
		asm("hlt");
		if(wait_var!=tmp) {
			return;
		}
	}
}

void isr_handler(void) {
	p("ISR!");
}

void irq0_handler(void) {
	if(wait_var==0) {
		wait_var=1;
	}
	else {
		wait_var=0;
	}

	time_cycle++;
	if(time_cycle>=10 && program!=STARTING) {
		draw_time();
		time_cycle=0;
	}

    outb(0x20, 0x20); //EOI
}

void irq1_handler(void) {
    if (inb(0x64) & 0x01) {
        keycode = inb(0x60);

		if(keycode==KEY_F10) {
			mouse_cycle=0;
		}

        if(keycode==KEY_CAPSLOCK) {
            if(shift==0) {
                shift=1;
            }
            else {
                shift=0;
            }
        }

        if(keycode==KEY_LEFT_SHIFT) {
            if(shift==0) {
                shift=1;
            }
            else {
                shift=0;
            }
        }
        
        if(keycode==(KEY_LEFT_SHIFT + 0x80)) {
            if(shift==0) {
                shift=1;
            }
            else {
                shift=0;
            }
        }
        
        //convert keycode to ascii
        key_ascii=0;
    	if(keycode==KEY_A) { key_ascii = 'a'; }
    	if(keycode==KEY_B) { key_ascii = 'b'; }
    	if(keycode==KEY_C) { key_ascii = 'c'; }
    	if(keycode==KEY_D) { key_ascii = 'd'; }
    	if(keycode==KEY_E) { key_ascii = 'e'; }
    	if(keycode==KEY_F) { key_ascii = 'f'; }
    	if(keycode==KEY_G) { key_ascii = 'g'; }
    	if(keycode==KEY_H) { key_ascii = 'h'; }
    	if(keycode==KEY_I) { key_ascii = 'i'; }
    	if(keycode==KEY_J) { key_ascii = 'j'; }
    	if(keycode==KEY_K) { key_ascii = 'k'; }
    	if(keycode==KEY_L) { key_ascii = 'l'; }
    	if(keycode==KEY_M) { key_ascii = 'm'; }
    	if(keycode==KEY_N) { key_ascii = 'n'; }
    	if(keycode==KEY_O) { key_ascii = 'o'; }
    	if(keycode==KEY_P) { key_ascii = 'p'; }
    	if(keycode==KEY_Q) { key_ascii = 'q'; }
    	if(keycode==KEY_R) { key_ascii = 'r'; }
    	if(keycode==KEY_S) { key_ascii = 's'; }
    	if(keycode==KEY_T) { key_ascii = 't'; }
    	if(keycode==KEY_U) { key_ascii = 'u'; }
    	if(keycode==KEY_V) { key_ascii = 'v'; }
    	if(keycode==KEY_W) { key_ascii = 'w'; }
    	if(keycode==KEY_X) { key_ascii = 'x'; }
    	if(keycode==KEY_Y) { key_ascii = 'y'; }
    	if(keycode==KEY_Z) { key_ascii = 'z'; }
    	if(keycode==KEY_1) { key_ascii = '1'; }
    	if(keycode==KEY_2) { key_ascii = '2'; }
    	if(keycode==KEY_3) { key_ascii = '3'; }
    	if(keycode==KEY_4) { key_ascii = '4'; }
    	if(keycode==KEY_5) { key_ascii = '5'; }
    	if(keycode==KEY_6) { key_ascii = '6'; }
    	if(keycode==KEY_7) { key_ascii = '7'; }
    	if(keycode==KEY_8) { key_ascii = '8'; }
    	if(keycode==KEY_9) { key_ascii = '9'; }
    	if(keycode==KEY_0) { key_ascii = '0'; }
    	if(keycode==KEY_SPACE) { key_ascii = ' '; }
    	if(shift==1 && key_ascii>= 'a' && key_ascii<= 'z') {
            key_ascii = (key_ascii - 32);
        }

		key_is_press(keycode, key_ascii);

    }

    outb(0x20, 0x20); //EOI
}

void irq2_handler(void) {
    outb(0x20, 0x20); //EOI

}

void irq3_handler(void) {
    outb(0x20, 0x20); //EOI

}

void irq4_handler(void) {
    outb(0x20, 0x20); //EOI

}

void irq5_handler(void) {
    outb(0x20, 0x20); //EOI

}

void irq6_handler(void) {
    outb(0x20, 0x20); //EOI

}

void irq7_handler(void) {
    outb(0x20, 0x20); //EOI

}

void irq8_handler(void) {
    outb(0xA0, 0x20);
    outb(0x20, 0x20); //EOI        
}

void irq9_handler(void) {
    outb(0xA0, 0x20);
    outb(0x20, 0x20); //EOI

}

void irq10_handler(void) {  //Sound card IRQ - Intel HD Audio
    outb(0xA0, 0x20);
    outb(0x20, 0x20); //EOI

}

void irq11_handler(void) {
    outb(0xA0, 0x20);
    outb(0x20, 0x20); //EOI
}

void irq12_handler(void) { 
	mouse_packet[mouse_cycle++] = inb(0x60);

	if(mouse_cycle>=3) {
		mouse_cycle = 0;

		if(handle_irq12==0) {
			handle_irq12=1;

    		if ((mouse_packet[0] & 0x80) || (mouse_packet[0] & 0x40)) {
				handle_irq12=0;

    			outb(0xA0, 0x20);
    			outb(0x20, 0x20); //EOI
        		return;
    		}
			
			if(mouse_cursor_visible==1) {
				handle_mouse_packet(mouse_packet[1], mouse_packet[2], mouse_packet[0], 0);
			}

			handle_irq12=0;
		}
	}

    outb(0xA0, 0x20);
    outb(0x20, 0x20); //EOI

}

void irq13_handler(void) {
    outb(0xA0, 0x20);
    outb(0x20, 0x20); //EOI

}

void irq14_handler(void) {
    //acknowledge interrupt
    inb(0x1F7);

    outb(0xA0, 0x20);
    outb(0x20, 0x20); //EOI
}

void irq15_handler(void) {
    //acknowledge interrupt
    inb(0x177);

	ide_secondary_interrupt=1;

    outb(0xA0, 0x20);
    outb(0x20, 0x20); //EOI
}
