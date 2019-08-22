//LIGHTNINGOS

#define WHITE 0xFFFF
#define BLACK 0x0000

uint32_t vesa_lfb=0;
uint16_t vesa_x=0;
uint16_t vesa_y=0;
uint16_t vesa_bpp=0;

uint8_t vesa_font[1220]; //array for 122 chars

uint32_t print_line=0;

void read_vesa_info(void) {
    uint32_t *vesablockd = (uint32_t *) 0x1000;  //position of Vesa Mode Info Block loaded by bootloader dword read
    uint16_t *vesablockw = (uint16_t *) 0x1000;  //position of Vesa Mode Info Block loaded by bootloader word read
    uint8_t *vesablockb = (uint8_t *) 0x1000;  //position of Vesa Mode Info Block loaded by bootloader byte read

    vesa_lfb=vesablockd[10];  //Linear Frame Buffer position

	vesa_x=vesablockw[9];
    vesa_y=vesablockw[10];
    vesa_bpp=vesablockb[23];
}

void pixel(uint32_t line, uint32_t column, uint16_t color) {
    uint16_t *vidmem = (uint16_t *) (vesa_lfb);  //start of frame buffer
    uint32_t i = ((line*vesa_x)+column);
    vidmem[i]=color;
}

uint16_t read_pixel(uint32_t line, uint32_t column) {
    uint16_t *vidmem = (uint16_t *) (vesa_lfb);  //start of frame buffer
    uint32_t i = ((line*vesa_x)+column);
    return (vidmem[i]);
}

void clear_screen(void) {
    uint16_t *vidmem = (uint16_t *) (vesa_lfb);  //start of frame buffer, for high speed
	uint32_t lenght=0;

	lenght=(vesa_x * vesa_y);

    for(int i=0; i<lenght; i++) {
        vidmem[i]=0xFFFF;  //white
    }
}

//Drawing functions
void draw_full_line(uint16_t line, uint16_t color) {
    for(int i=0; i<800; i++) {
        pixel(line, i, color);
    }
}

void draw_full_column(uint16_t column, uint16_t color) {
    for(int i=0; i<600; i++) {
        pixel(i, column, color);
    }
}

void draw_line(uint16_t line, uint16_t column, uint16_t lenght, uint16_t color) {
    for(int i=0; i<lenght; i++) {
        pixel(line, column, color);
        column++;   //next column
    }
}

void draw_column(uint16_t line, uint16_t column, uint16_t height, uint16_t color) {
    for(int i=0; i<height; i++) {
        pixel(line, column, color);
        line++;    //next line
    }
}

void draw_square(uint16_t line, uint16_t column, uint16_t lenght, uint16_t height, uint16_t color) {
    for(int i=0; i<height; i++) {
        for(int j=0; j<lenght; j++) { 
            pixel( (line+i), (column+j), color);
        }
    }
}

void draw_empty_square(uint16_t line, uint16_t column, uint16_t lenght, uint16_t height, uint16_t color) {
	draw_line(line, column, lenght, color);
	draw_line( (line+height), column, lenght, color);
	draw_column(line, column, height, color);
	draw_column(line, (column+lenght), height, color);
	pixel((line+height), (column+lenght), color);
}

void print(uint8_t *message, uint32_t line, uint32_t column, uint16_t color) {
    uint32_t char_position=0;
    uint16_t char_color=0;

    //emulate text mode
    line = (line * 10);
    column = (column * 8);

    while(*message!=0) {
        char_position=*message;  //get char from message
        char_position=(char_position*10);  //calculate position of char in font array

        for(int font_line=0; font_line<10; font_line++) {  //write char lines
            //pixel 0
            if( (vesa_font[char_position+font_line] & 0x80) == 0x80) {
                pixel( (line+font_line), column, color);
            }

            //pixel 1
            if( (vesa_font[char_position+font_line] & 0x40) == 0x40) {
                pixel( (line+font_line), (column+1), color);
            }

            //pixel 2
            if( (vesa_font[char_position+font_line] & 0x20) == 0x20) {
                pixel( (line+font_line), (column+2), color);
            }

            //pixel 3
            if( (vesa_font[char_position+font_line] & 0x10) == 0x10) {
                pixel( (line+font_line), (column+3), color);
            } 

            //pixel 4
            if( (vesa_font[char_position+font_line] & 0x08) == 0x08) {
                pixel( (line+font_line), (column+4), color);
            } 

            //pixel 5
            if( (vesa_font[char_position+font_line] & 0x04) == 0x04) {
                pixel( (line+font_line), (column+5), color);
            }

            //pixel 6
            if( (vesa_font[char_position+font_line] & 0x02) == 0x02) {
                pixel( (line+font_line), (column+6), color);
            }

            //pixel 7
            if( (vesa_font[char_position+font_line] & 0x01) == 0x01) {
                pixel( (line+font_line), (column+7), color);
            }
        }

        column=column+8;  //get position for next char
        *message++;
    }

}

void print_var(uint64_t n, unsigned int line, unsigned int column, unsigned int colour) {

    if (n == 0)
    {
        print("0", line, column, colour);
        return;
    }

    long acc = n;
    char c[32];
    long i = 0;
    while (acc > 0)
    {
        c[i] = '0' + acc%10;
        acc /= 10;
        i++;
    }
    c[i] = 0;

    char c2[32];
    c2[i--] = 0;
    long j = 0;
    while(i >= 0)
    {
        c2[i--] = c[j++];
    }

    print(c2, line, column, colour);

}

void print_hex(uint32_t n, unsigned int line, unsigned int column, unsigned int colour){
    uint8_t c[10];
    c[0]=('0');
    c[1]=('x');
    uint8_t digit=0;
    for (int i = 0; i < 8; ++i) {
        digit = (n >> (32 - (i * 4) - 4)) & 0xF;
        if (digit < 0xA) {
            c[i+2]=('0' + digit);
        }
        else {
            c[i+2]=('A' + (digit - 0xA));
        }
    }
    c[10]=0;  //end of string

    print(c, line, column, colour);
}

void init_vesa_font(void) {
    //init all array
    for(uint32_t i=0; i<1220; i++) {
        vesa_font[i]=0xFF;
    }

    //A
    vesa_font[('A'*10)+0]=0x00;  //........ 0x00
    vesa_font[('A'*10)+1]=0x18;  //...00... 0x18
    vesa_font[('A'*10)+2]=0x3C;  //..0000.. 0x3C
    vesa_font[('A'*10)+3]=0x66;  //.00..00. 0x66
    vesa_font[('A'*10)+4]=0x66;  //.00..00. 0x66
    vesa_font[('A'*10)+5]=0x7E;  //.000000. 0x7E
    vesa_font[('A'*10)+6]=0x66;  //.00..00. 0x66
    vesa_font[('A'*10)+7]=0x66;  //.00..00. 0x66
    vesa_font[('A'*10)+8]=0x66;  //.00..00. 0x66
    vesa_font[('A'*10)+9]=0x00;  //........ 0x00

    //B
    vesa_font[('B'*10)+0]=0x00;  //........ 0x00
    vesa_font[('B'*10)+1]=0x7C;  //.00000.. 0x7C
    vesa_font[('B'*10)+2]=0x7E;  //.000000. 0x7E
    vesa_font[('B'*10)+3]=0x66;  //.00..00. 0x66
    vesa_font[('B'*10)+4]=0x7C;  //.00000.. 0x7C
    vesa_font[('B'*10)+5]=0x7C;  //.00000.. 0x7C
    vesa_font[('B'*10)+6]=0x66;  //.00..00. 0x66
    vesa_font[('B'*10)+7]=0x7E;  //.000000. 0x7E
    vesa_font[('B'*10)+8]=0x7C;  //.00000.. 0x7C
    vesa_font[('B'*10)+9]=0x00;  //........ 0x00

    //C
    vesa_font[('C'*10)+0]=0x00;  //........ 0x00
    vesa_font[('C'*10)+1]=0x1C;  //...000.. 0x1C
    vesa_font[('C'*10)+2]=0x3E;  //..00000. 0x3E
    vesa_font[('C'*10)+3]=0x76;  //.000.00. 0x76
    vesa_font[('C'*10)+4]=0x60;  //.00..... 0x60
    vesa_font[('C'*10)+5]=0x60;  //.00..... 0x60
    vesa_font[('C'*10)+6]=0x76;  //.000.00. 0x76
    vesa_font[('C'*10)+7]=0x3E;  //..00000. 0x3E
    vesa_font[('C'*10)+8]=0x1C;  //...000.. 0x1C
    vesa_font[('C'*10)+9]=0x00;  //........ 0x00

    //D
    vesa_font[('D'*10)+0]=0x00;  //........ 0x00
    vesa_font[('D'*10)+1]=0x7C;  //.00000.. 0x7C
    vesa_font[('D'*10)+2]=0x7E;  //.000000. 0x7E
    vesa_font[('D'*10)+3]=0x66;  //.00..00. 0x66
    vesa_font[('D'*10)+4]=0x66;  //.00..00. 0x66
    vesa_font[('D'*10)+5]=0x66;  //.00..00. 0x66
    vesa_font[('D'*10)+6]=0x66;  //.00..00. 0x66
    vesa_font[('D'*10)+7]=0x7E;  //.000000. 0x7E
    vesa_font[('D'*10)+8]=0x7C;  //.00000.. 0x7C
    vesa_font[('D'*10)+9]=0x00;  //........ 0x00

    //E
    vesa_font[('E'*10)+0]=0x00;  //........ 0x00
    vesa_font[('E'*10)+1]=0x7E;  //.000000. 0x7E
    vesa_font[('E'*10)+2]=0x7E;  //.000000. 0x7E
    vesa_font[('E'*10)+3]=0x60;  //.00..... 0x60
    vesa_font[('E'*10)+4]=0x7E;  //.000000. 0x7E
    vesa_font[('E'*10)+5]=0x7E;  //.000000. 0x7E
    vesa_font[('E'*10)+6]=0x60;  //.00..... 0x60
    vesa_font[('E'*10)+7]=0x7E;  //.000000. 0x7E
    vesa_font[('E'*10)+8]=0x7E;  //.000000. 0x7E
    vesa_font[('E'*10)+9]=0x00;  //........ 0x00

    //F
    vesa_font[('F'*10)+0]=0x00;  //........ 0x00
    vesa_font[('F'*10)+1]=0x7E;  //.000000. 0x7E
    vesa_font[('F'*10)+2]=0x7E;  //.000000. 0x7E
    vesa_font[('F'*10)+3]=0x60;  //.00..... 0x60
    vesa_font[('F'*10)+4]=0x7E;  //.000000. 0x7E
    vesa_font[('F'*10)+5]=0x7E;  //.000000. 0x7E
    vesa_font[('F'*10)+6]=0x60;  //.00..... 0x60
    vesa_font[('F'*10)+7]=0x60;  //.00..... 0x60
    vesa_font[('F'*10)+8]=0x60;  //.00..... 0x60
    vesa_font[('F'*10)+9]=0x00;  //........ 0x00

    //G
    vesa_font[('G'*10)+0]=0x00;  //........ 0x00
    vesa_font[('G'*10)+1]=0x3C;  //..0000.. 0x3C
    vesa_font[('G'*10)+2]=0x7E;  //.000000. 0x7E
    vesa_font[('G'*10)+3]=0x66;  //.00..00. 0x66
    vesa_font[('G'*10)+4]=0x60;  //.00..... 0x60
    vesa_font[('G'*10)+5]=0x6E;  //.00.000. 0x6E
    vesa_font[('G'*10)+6]=0x66;  //.00..00. 0x66
    vesa_font[('G'*10)+7]=0x7E;  //.000000. 0x7E
    vesa_font[('G'*10)+8]=0x3C;  //..0000.. 0x3C
    vesa_font[('G'*10)+9]=0x00;  //........ 0x00

    //H
    vesa_font[('H'*10)+0]=0x00;  //........ 0x00
    vesa_font[('H'*10)+1]=0x66;  //.00..00. 0x66
    vesa_font[('H'*10)+2]=0x66;  //.00..00. 0x66
    vesa_font[('H'*10)+3]=0x66;  //.00..00. 0x66
    vesa_font[('H'*10)+4]=0x7E;  //.000000. 0x7E
    vesa_font[('H'*10)+5]=0x7E;  //.000000. 0x7E
    vesa_font[('H'*10)+6]=0x66;  //.00..00. 0x66
    vesa_font[('H'*10)+7]=0x66;  //.00..00. 0x66
    vesa_font[('H'*10)+8]=0x66;  //.00..00. 0x66
    vesa_font[('H'*10)+9]=0x00;  //........ 0x00

    //I
    vesa_font[('I'*10)+0]=0x00;  //........ 0x00
    vesa_font[('I'*10)+1]=0x3C;  //..0000.. 0x3C
    vesa_font[('I'*10)+2]=0x18;  //...00... 0x18
    vesa_font[('I'*10)+3]=0x18;  //...00... 0x18
    vesa_font[('I'*10)+4]=0x18;  //...00... 0x18
    vesa_font[('I'*10)+5]=0x18;  //...00... 0x18
    vesa_font[('I'*10)+6]=0x18;  //...00... 0x18
    vesa_font[('I'*10)+7]=0x18;  //...00... 0x18
    vesa_font[('I'*10)+8]=0x3C;  //..0000.. 0x3C
    vesa_font[('I'*10)+9]=0x00;  //........ 0x00

    //J
    vesa_font[('J'*10)+0]=0x00;  //........ 0x00
    vesa_font[('J'*10)+1]=0x06;  //.....00. 0x06
    vesa_font[('J'*10)+2]=0x06;  //.....00. 0x06
    vesa_font[('J'*10)+3]=0x06;  //.....00. 0x06
    vesa_font[('J'*10)+4]=0x06;  //.....00. 0x06
    vesa_font[('J'*10)+5]=0x66;  //.00..00. 0x66
    vesa_font[('J'*10)+6]=0x66;  //.00..00. 0x66
    vesa_font[('J'*10)+7]=0x7E;  //.000000. 0x7E
    vesa_font[('J'*10)+8]=0x3C;  //..0000.. 0x3C
    vesa_font[('J'*10)+9]=0x00;  //........ 0x00

    //K
    vesa_font[('K'*10)+0]=0x00;  //........ 0x00
    vesa_font[('K'*10)+1]=0x66;  //.00..00. 0x66
    vesa_font[('K'*10)+2]=0x6C;  //.00.00.. 0x6C
    vesa_font[('K'*10)+3]=0x78;  //.0000... 0x78
    vesa_font[('K'*10)+4]=0x70;  //.000.... 0x70
    vesa_font[('K'*10)+5]=0x78;  //.0000... 0x78
    vesa_font[('K'*10)+6]=0x6C;  //.00.00.. 0x6C
    vesa_font[('K'*10)+7]=0x66;  //.00..00. 0x66
    vesa_font[('K'*10)+8]=0x66;  //.00..00. 0x66
    vesa_font[('K'*10)+9]=0x00;  //........ 0x00

    //L
    vesa_font[('L'*10)+0]=0x00;  //........ 0x00
    vesa_font[('L'*10)+1]=0x60;  //.00..... 0x60
    vesa_font[('L'*10)+2]=0x60;  //.00..... 0x60
    vesa_font[('L'*10)+3]=0x60;  //.00..... 0x60
    vesa_font[('L'*10)+4]=0x60;  //.00..... 0x60
    vesa_font[('L'*10)+5]=0x60;  //.00..... 0x60
    vesa_font[('L'*10)+6]=0x60;  //.00..... 0x60
    vesa_font[('L'*10)+7]=0x7E;  //.000000. 0x7E
    vesa_font[('L'*10)+8]=0x7E;  //.000000. 0x7E
    vesa_font[('L'*10)+9]=0x00;  //........ 0x00

    //M
    vesa_font[('M'*10)+0]=0x00;  //........ 0x00
    vesa_font[('M'*10)+1]=0xC3;  //00....00 0xC3 
    vesa_font[('M'*10)+2]=0xE7;  //000..000 0xE7
    vesa_font[('M'*10)+3]=0xFF;  //00000000 0xFF
    vesa_font[('M'*10)+4]=0xDB;  //00.00.00 0xDB
    vesa_font[('M'*10)+5]=0xC3;  //00....00 0xC3
    vesa_font[('M'*10)+6]=0xC3;  //00....00 0xC3
    vesa_font[('M'*10)+7]=0xC3;  //00....00 0xC3
    vesa_font[('M'*10)+8]=0xC3;  //00....00 0xC3
    vesa_font[('M'*10)+9]=0x00;  //........ 0x00

    //N
    vesa_font[('N'*10)+0]=0x00;  //........ 0x00
    vesa_font[('N'*10)+1]=0x66;  //.00..00. 0x66
    vesa_font[('N'*10)+2]=0x66;  //.00..00. 0x66
    vesa_font[('N'*10)+3]=0x76;  //.000.00. 0x76
    vesa_font[('N'*10)+4]=0x76;  //.000.00. 0x76
    vesa_font[('N'*10)+5]=0x7E;  //.000000. 0x7E
    vesa_font[('N'*10)+6]=0x6E;  //.00.000. 0x6E
    vesa_font[('N'*10)+7]=0x6E;  //.00.000. 0x6E
    vesa_font[('N'*10)+8]=0x66;  //.00..00. 0x66
    vesa_font[('N'*10)+9]=0x00;  //........ 0x00

    //O
    vesa_font[('O'*10)+0]=0x00;  //........ 0x00
    vesa_font[('O'*10)+1]=0x18;  //...00... 0x18
    vesa_font[('O'*10)+2]=0x3C;  //..0000.. 0x3C
    vesa_font[('O'*10)+3]=0x66;  //.00..00. 0x66
    vesa_font[('O'*10)+4]=0x66;  //.00..00. 0x66
    vesa_font[('O'*10)+5]=0x66;  //.00..00. 0x66
    vesa_font[('O'*10)+6]=0x66;  //.00..00. 0x66
    vesa_font[('O'*10)+7]=0x3C;  //..0000.. 0x3C
    vesa_font[('O'*10)+8]=0x18;  //...00... 0x18
    vesa_font[('O'*10)+9]=0x00;  //........ 0x00

    //P
    vesa_font[('P'*10)+0]=0x00;  //........ 0x00
    vesa_font[('P'*10)+1]=0x7C;  //.00000.. 0x7C
    vesa_font[('P'*10)+2]=0x7E;  //.000000. 0x7E
    vesa_font[('P'*10)+3]=0x66;  //.00..00. 0x66
    vesa_font[('P'*10)+4]=0x7E;  //.000000. 0x7E
    vesa_font[('P'*10)+5]=0x7C;  //.00000.. 0x7C
    vesa_font[('P'*10)+6]=0x60;  //.00..... 0x60
    vesa_font[('P'*10)+7]=0x60;  //.00..... 0x60
    vesa_font[('P'*10)+8]=0x60;  //.00..... 0x60
    vesa_font[('P'*10)+9]=0x00;  //........ 0x00

    //Q
    vesa_font[('Q'*10)+0]=0x00;  //........ 0x00
    vesa_font[('Q'*10)+1]=0x18;  //...00... 0x18
    vesa_font[('Q'*10)+2]=0x3C;  //..0000.. 0x3C
    vesa_font[('Q'*10)+3]=0x66;  //.000000. 0x7E
    vesa_font[('Q'*10)+4]=0x66;  //.00..00. 0x66
    vesa_font[('Q'*10)+5]=0x66;  //.00..00. 0x66
    vesa_font[('Q'*10)+6]=0x7E;  //.000000. 0x7E
    vesa_font[('Q'*10)+7]=0x3C;  //..0000.. 0x3C
    vesa_font[('Q'*10)+8]=0x06;  //.....00. 0x06
    vesa_font[('Q'*10)+9]=0x00;  //........ 0x00

    //R
    vesa_font[('R'*10)+0]=0x00;  //........ 0x00
    vesa_font[('R'*10)+1]=0x7C;  //.00000.. 0x7C
    vesa_font[('R'*10)+2]=0x7E;  //.000000. 0x7E
    vesa_font[('R'*10)+3]=0x66;  //.00..00. 0x66
    vesa_font[('R'*10)+4]=0x7E;  //.000000. 0x7E
    vesa_font[('R'*10)+5]=0x7C;  //.00000.. 0x7C
    vesa_font[('R'*10)+6]=0x78;  //.0000... 0x78
    vesa_font[('R'*10)+7]=0x6C;  //.00.00.. 0x6C
    vesa_font[('R'*10)+8]=0x6E;  //.00.000. 0x6E
    vesa_font[('R'*10)+9]=0x00;  //........ 0x00

    //S
    vesa_font[('S'*10)+0]=0x00;  //........ 0x00
    vesa_font[('S'*10)+1]=0x3C;  //..0000.. 0x3C
    vesa_font[('S'*10)+2]=0x7E;  //.000000. 0x7E
    vesa_font[('S'*10)+3]=0x60;  //.00..... 0x60
    vesa_font[('S'*10)+4]=0x7E;  //.000000. 0x7E
    vesa_font[('S'*10)+5]=0x7E;  //.000000. 0x7E
    vesa_font[('S'*10)+6]=0x06;  //.....00. 0x06
    vesa_font[('S'*10)+7]=0x7E;  //.000000. 0x7E
    vesa_font[('S'*10)+8]=0x3C;  //..0000.. 0x3C
    vesa_font[('S'*10)+9]=0x00;  //........ 0x00

    //T
    vesa_font[('T'*10)+0]=0x00;  //........ 0x00
    vesa_font[('T'*10)+1]=0x7E;  //.000000. 0x7E
    vesa_font[('T'*10)+2]=0x7E;  //.000000. 0x7E
    vesa_font[('T'*10)+3]=0x18;  //...00... 0x18
    vesa_font[('T'*10)+4]=0x18;  //...00... 0x18
    vesa_font[('T'*10)+5]=0x18;  //...00... 0x18
    vesa_font[('T'*10)+6]=0x18;  //...00... 0x18
    vesa_font[('T'*10)+7]=0x18;  //...00... 0x18
    vesa_font[('T'*10)+8]=0x18;  //...00... 0x18
    vesa_font[('T'*10)+9]=0x00;  //........ 0x00

    //U
    vesa_font[('U'*10)+0]=0x00;  //........ 0x00
    vesa_font[('U'*10)+1]=0x66;  //.00..00. 0x66
    vesa_font[('U'*10)+2]=0x66;  //.00..00. 0x66
    vesa_font[('U'*10)+3]=0x66;  //.00..00. 0x66
    vesa_font[('U'*10)+4]=0x66;  //.00..00. 0x66
    vesa_font[('U'*10)+5]=0x66;  //.00..00. 0x66
    vesa_font[('U'*10)+6]=0x66;  //.00..00. 0x66
    vesa_font[('U'*10)+7]=0x7E;  //.000000. 0x7E
    vesa_font[('U'*10)+8]=0x3C;  //..0000.. 0x3C
    vesa_font[('U'*10)+9]=0x00;  //........ 0x00

    //V
    vesa_font[('V'*10)+0]=0x00;  //........ 0x00
    vesa_font[('V'*10)+1]=0x66;  //.00..00. 0x66
    vesa_font[('V'*10)+2]=0x66;  //.00..00. 0x66
    vesa_font[('V'*10)+3]=0x66;  //.00..00. 0x66
    vesa_font[('V'*10)+4]=0x66;  //.00..00. 0x66
    vesa_font[('V'*10)+5]=0x66;  //.00..00. 0x66
    vesa_font[('V'*10)+6]=0x7E;  //.000000. 0x7E
    vesa_font[('V'*10)+7]=0x3C;  //..0000.. 0x3C
    vesa_font[('V'*10)+8]=0x18;  //...00... 0x18
    vesa_font[('V'*10)+9]=0x00;  //........ 0x00

    //W
    vesa_font[('W'*10)+0]=0x00;  //........ 0x00
    vesa_font[('W'*10)+1]=0xDB;  //00.00.00 0xDB
    vesa_font[('W'*10)+2]=0xDB;  //00.00.00 0xDB
    vesa_font[('W'*10)+3]=0xDB;  //00.00.00 0xDB
    vesa_font[('W'*10)+4]=0xDB;  //00.00.00 0xDB
    vesa_font[('W'*10)+5]=0xDB;  //00.00.00 0xDB
    vesa_font[('W'*10)+6]=0xDB;  //00.00.00 0xDB
    vesa_font[('W'*10)+7]=0xFF;  //00000000 0xFF
    vesa_font[('W'*10)+8]=0x66;  //.00..00. 0x66
    vesa_font[('W'*10)+9]=0x00;  //........ 0x00

    //X
    vesa_font[('X'*10)+0]=0x00;  //........ 0x00
    vesa_font[('X'*10)+1]=0x66;  //.00..00. 0x66
    vesa_font[('X'*10)+2]=0x66;  //.00..00. 0x66
    vesa_font[('X'*10)+3]=0x3C;  //..0000.. 0x3C
    vesa_font[('X'*10)+4]=0x18;  //...00... 0x18
    vesa_font[('X'*10)+5]=0x18;  //...00... 0x18
    vesa_font[('X'*10)+6]=0x3C;  //..0000.. 0x3C
    vesa_font[('X'*10)+7]=0x66;  //.00..00. 0x66
    vesa_font[('X'*10)+8]=0x66;  //.00..00. 0x66
    vesa_font[('X'*10)+9]=0x00;  //........ 0x00

    //Y
    vesa_font[('Y'*10)+0]=0x00;  //........ 0x00
    vesa_font[('Y'*10)+1]=0x66;  //.00..00. 0x66
    vesa_font[('Y'*10)+2]=0x66;  //.00..00. 0x66
    vesa_font[('Y'*10)+3]=0x66;  //.00..00. 0x66
    vesa_font[('Y'*10)+4]=0x7E;  //.000000. 0x7E
    vesa_font[('Y'*10)+5]=0x3C;  //..0000.. 0x3C
    vesa_font[('Y'*10)+6]=0x18;  //...00... 0x18
    vesa_font[('Y'*10)+7]=0x18;  //...00... 0x18
    vesa_font[('Y'*10)+8]=0x18;  //...00... 0x18
    vesa_font[('Y'*10)+9]=0x00;  //........ 0x00

    //Z
    vesa_font[('Z'*10)+0]=0x00;  //........ 0x00
    vesa_font[('Z'*10)+1]=0x7E;  //.000000. 0x7E
    vesa_font[('Z'*10)+2]=0x7E;  //.000000. 0x7E
    vesa_font[('Z'*10)+3]=0x0E;  //....000. 0x0E
    vesa_font[('Z'*10)+4]=0x1C;  //...000.. 0x1C 
    vesa_font[('Z'*10)+5]=0x38;  //..000... 0x38
    vesa_font[('Z'*10)+6]=0x70;  //.000.... 0x70
    vesa_font[('Z'*10)+7]=0x7E;  //.000000. 0x7E
    vesa_font[('Z'*10)+8]=0x7E;  //.000000. 0x7E
    vesa_font[('Z'*10)+9]=0x00;  //........ 0x00

    //a
    vesa_font[('a'*10)+0]=0x00;  //........ 0x00
    vesa_font[('a'*10)+1]=0x00;  //........ 0x00
    vesa_font[('a'*10)+2]=0x00;  //........ 0x00
    vesa_font[('a'*10)+3]=0x3C;  //..0000.. 0x3C
    vesa_font[('a'*10)+4]=0x7E;  //.000000. 0x7E
    vesa_font[('a'*10)+5]=0x06;  //.....00. 0x06
    vesa_font[('a'*10)+6]=0x3E;  //..00000. 0x3E
    vesa_font[('a'*10)+7]=0x66;  //.00..00. 0x66
    vesa_font[('a'*10)+8]=0x3E;  //..00000. 0x3E
    vesa_font[('a'*10)+9]=0x00;  //........ 0x00

    //b
    vesa_font[('b'*10)+0]=0x00;  //........ 0x00
    vesa_font[('b'*10)+1]=0x60;  //.00..... 0x60
    vesa_font[('b'*10)+2]=0x60;  //.00..... 0x60
    vesa_font[('b'*10)+3]=0x60;  //.00..... 0x60
    vesa_font[('b'*10)+4]=0x7C;  //.00000.. 0x7C
    vesa_font[('b'*10)+5]=0x7E;  //.000000. 0x7E
    vesa_font[('b'*10)+6]=0x66;  //.00..00. 0x66
    vesa_font[('b'*10)+7]=0x7E;  //.000000. 0x7E
    vesa_font[('b'*10)+8]=0x7C;  //.00000.. 0x7C
    vesa_font[('b'*10)+9]=0x00;  //........ 0x00

    //c
    vesa_font[('c'*10)+0]=0x00;  //........ 0x00
    vesa_font[('c'*10)+1]=0x00;  //........ 0x00
    vesa_font[('c'*10)+2]=0x00;  //........ 0x00
    vesa_font[('c'*10)+3]=0x3C;  //..0000.. 0x3C
    vesa_font[('c'*10)+4]=0x7E;  //.000000. 0x7E
    vesa_font[('c'*10)+5]=0x60;  //.00..... 0x60
    vesa_font[('c'*10)+6]=0x60;  //.00..... 0x60
    vesa_font[('c'*10)+7]=0x7E;  //.000000. 0x7E
    vesa_font[('c'*10)+8]=0x3C;  //..0000.. 0x3C
    vesa_font[('c'*10)+9]=0x00;  //........ 0x00

    //d
    vesa_font[('d'*10)+0]=0x00;  //........ 0x00
    vesa_font[('d'*10)+1]=0x06;  //.....00. 0x06
    vesa_font[('d'*10)+2]=0x06;  //.....00. 0x06
    vesa_font[('d'*10)+3]=0x06;  //.....00. 0x06
    vesa_font[('d'*10)+4]=0x3E;  //..00000. 0x3E
    vesa_font[('d'*10)+5]=0x7E;  //.000000. 0x7E
    vesa_font[('d'*10)+6]=0x66;  //.00..00. 0x66
    vesa_font[('d'*10)+7]=0x7E;  //.000000. 0x7E
    vesa_font[('d'*10)+8]=0x3E;  //..00000. 0x3E
    vesa_font[('d'*10)+9]=0x00;  //........ 0x00

    //e
    vesa_font[('e'*10)+0]=0x00;  //........ 0x00
    vesa_font[('e'*10)+1]=0x00;  //........ 0x00
    vesa_font[('e'*10)+2]=0x00;  //........ 0x00
    vesa_font[('e'*10)+3]=0x3C;  //..0000.. 0x3C
    vesa_font[('e'*10)+4]=0x7E;  //.000000. 0x7E
    vesa_font[('e'*10)+5]=0x66;  //.00..00. 0x66
    vesa_font[('e'*10)+6]=0x7E;  //.000000. 0x7E
    vesa_font[('e'*10)+7]=0x60;  //.00..... 0x60
    vesa_font[('e'*10)+8]=0x3C;  //..0000.. 0x3C
    vesa_font[('e'*10)+9]=0x00;  //........ 0x00

    //f
    vesa_font[('f'*10)+0]=0x00;  //........ 0x00
    vesa_font[('f'*10)+1]=0x3C;  //..0000.. 0x3C
    vesa_font[('f'*10)+2]=0x7E;  //.000000. 0x7E
    vesa_font[('f'*10)+3]=0x66;  //.00..00. 0x66
    vesa_font[('f'*10)+4]=0x60;  //.00..... 0x60
    vesa_font[('f'*10)+5]=0xF8;  //00000... 0xF8
    vesa_font[('f'*10)+6]=0x60;  //.00..... 0x60
    vesa_font[('f'*10)+7]=0x60;  //.00..... 0x60
    vesa_font[('f'*10)+8]=0x60;  //.00..... 0x60
    vesa_font[('f'*10)+9]=0x00;  //........ 0x00

    //g
    vesa_font[('g'*10)+0]=0x00;  //........ 0x00
    vesa_font[('g'*10)+1]=0x00;  //........ 0x00
    vesa_font[('g'*10)+2]=0x00;  //........ 0x00
    vesa_font[('g'*10)+3]=0x3C;  //..0000.. 0x3C
    vesa_font[('g'*10)+4]=0x7E;  //.000000. 0x7E
    vesa_font[('g'*10)+5]=0x66;  //.00..00. 0x66
    vesa_font[('g'*10)+6]=0x3E;  //..00000. 0x3E
    vesa_font[('g'*10)+7]=0x06;  //.....00. 0x06
    vesa_font[('g'*10)+8]=0x7E;  //.000000. 0x7E
    vesa_font[('g'*10)+9]=0x3C;  //..0000.. 0x3C

    //h
    vesa_font[('h'*10)+0]=0x00;  //........ 0x00
    vesa_font[('h'*10)+1]=0x60;  //.00..... 0x60
    vesa_font[('h'*10)+2]=0x60;  //.00..... 0x60
    vesa_font[('h'*10)+3]=0x6C;  //.00.00.. 0x6C
    vesa_font[('h'*10)+4]=0x7E;  //.000000. 0x7E
    vesa_font[('h'*10)+5]=0x76;  //.000.00. 0x76
    vesa_font[('h'*10)+6]=0x66;  //.00..00. 0x66
    vesa_font[('h'*10)+7]=0x66;  //.00..00. 0x66
    vesa_font[('h'*10)+8]=0x66;  //.00..00. 0x66
    vesa_font[('h'*10)+9]=0x00;  //........ 0x00

    //i
    vesa_font[('i'*10)+0]=0x00;  //........ 0x00
    vesa_font[('i'*10)+1]=0x00;  //........ 0x00
    vesa_font[('i'*10)+2]=0x18;  //...00... 0x18
    vesa_font[('i'*10)+3]=0x18;  //...00... 0x18
    vesa_font[('i'*10)+4]=0x00;  //........ 0x00
    vesa_font[('i'*10)+5]=0x18;  //...00... 0x18
    vesa_font[('i'*10)+6]=0x18;  //...00... 0x18
    vesa_font[('i'*10)+7]=0x18;  //...00... 0x18
    vesa_font[('i'*10)+8]=0x18;  //...00... 0x18
    vesa_font[('i'*10)+9]=0x00;  //........ 0x00

    //j
    vesa_font[('j'*10)+0]=0x00;  //........ 0x00
    vesa_font[('j'*10)+1]=0x0C;  //....00.. 0x0C
    vesa_font[('j'*10)+2]=0x0C;  //....00.. 0x0C
    vesa_font[('j'*10)+3]=0x00;  //........ 0x00
    vesa_font[('j'*10)+4]=0x0C;  //....00.. 0x0C
    vesa_font[('j'*10)+5]=0x0C;  //....00.. 0x0C
    vesa_font[('j'*10)+6]=0x6C;  //.00.00.. 0x4C
    vesa_font[('j'*10)+7]=0x6C;  //.00.00.. 0x6C
    vesa_font[('j'*10)+8]=0x7C;  //.00000.. 0x7C
    vesa_font[('j'*10)+9]=0x38;  //..000... 0x38

    //k
    vesa_font[('k'*10)+0]=0x00;  //........ 0x00
    vesa_font[('k'*10)+1]=0x60;  //.00..... 0x60
    vesa_font[('k'*10)+2]=0x60;  //.00..... 0x60
    vesa_font[('k'*10)+3]=0x6E;  //.00.000. 0x6E
    vesa_font[('k'*10)+4]=0x6C;  //.00.00.. 0x6C
    vesa_font[('k'*10)+5]=0x78;  //.0000... 0x78
    vesa_font[('k'*10)+6]=0x78;  //.0000... 0x78
    vesa_font[('k'*10)+7]=0x6C;  //.00.00.. 0x6C
    vesa_font[('k'*10)+8]=0x6E;  //.00.000. 0x6E
    vesa_font[('k'*10)+9]=0x00;  //........ 0x00

    //l
    vesa_font[('l'*10)+0]=0x00;  //........ 0x00
    vesa_font[('l'*10)+1]=0x38;  //..000... 0x38
    vesa_font[('l'*10)+2]=0x38;  //..000... 0x38
    vesa_font[('l'*10)+3]=0x18;  //...00... 0x18
    vesa_font[('l'*10)+4]=0x18;  //...00... 0x18
    vesa_font[('l'*10)+5]=0x18;  //...00... 0x18
    vesa_font[('l'*10)+6]=0x18;  //...00... 0x18
    vesa_font[('l'*10)+7]=0x3C;  //..0000.. 0x3C
    vesa_font[('l'*10)+8]=0x3C;  //..0000.. 0x3C
    vesa_font[('l'*10)+9]=0x00;  //........ 0x00

    //m
    vesa_font[('m'*10)+0]=0x00;  //........ 0x00
    vesa_font[('m'*10)+1]=0x00;  //........ 0x00
    vesa_font[('m'*10)+2]=0x00;  //........ 0x00
    vesa_font[('m'*10)+3]=0xB6;  //0.00.00. 0xB6
    vesa_font[('m'*10)+4]=0xFF;  //00000000 0xFF
    vesa_font[('m'*10)+5]=0xDB;  //00.00.00 0xDB
    vesa_font[('m'*10)+6]=0xDB;  //00.00.00 0xDB
    vesa_font[('m'*10)+7]=0xDB;  //00.00.00 0xDB
    vesa_font[('m'*10)+8]=0xDB;  //00.00.00 0xDB
    vesa_font[('m'*10)+9]=0x00;  //........ 0x00

    //n
    vesa_font[('n'*10)+0]=0x00;  //........ 0x00
    vesa_font[('n'*10)+1]=0x00;  //........ 0x00
    vesa_font[('n'*10)+2]=0x00;  //........ 0x00
    vesa_font[('n'*10)+3]=0x6C;  //.00.00.. 0x6C
    vesa_font[('n'*10)+4]=0x7E;  //.000000. 0x7E
    vesa_font[('n'*10)+5]=0x76;  //.000.00. 0x76
    vesa_font[('n'*10)+6]=0x66;  //.00..00. 0x66
    vesa_font[('n'*10)+7]=0x66;  //.00..00. 0x66
    vesa_font[('n'*10)+8]=0x66;  //.00..00. 0x66
    vesa_font[('n'*10)+9]=0x00;  //........ 0x00

    //o
    vesa_font[('o'*10)+0]=0x00;  //........ 0x00
    vesa_font[('o'*10)+1]=0x00;  //........ 0x00
    vesa_font[('o'*10)+2]=0x00;  //........ 0x00
    vesa_font[('o'*10)+3]=0x3C;  //..0000.. 0x3C
    vesa_font[('o'*10)+4]=0x7E;  //.000000. 0x7E
    vesa_font[('o'*10)+5]=0x66;  //.00..00. 0x66
    vesa_font[('o'*10)+6]=0x66;  //.00..00. 0x66
    vesa_font[('o'*10)+7]=0x7E;  //.000000. 0x7E
    vesa_font[('o'*10)+8]=0x3C;  //..0000.. 0x3C
    vesa_font[('o'*10)+9]=0x00;  //........ 0x00

    //p
    vesa_font[('p'*10)+0]=0x00;  //........ 0x00
    vesa_font[('p'*10)+1]=0x00;  //........ 0x00
    vesa_font[('p'*10)+2]=0x00;  //........ 0x00
    vesa_font[('p'*10)+3]=0x7C;  //.00000.. 0x7C
    vesa_font[('p'*10)+4]=0x7E;  //.000000. 0x7E
    vesa_font[('p'*10)+5]=0x66;  //.00..00. 0x66
    vesa_font[('p'*10)+6]=0x7E;  //.000000. 0x7E
    vesa_font[('p'*10)+7]=0x7C;  //.00000.. 0x7C
    vesa_font[('p'*10)+8]=0x60;  //.00..... 0x60
    vesa_font[('p'*10)+9]=0x60;  //.00..... 0x60

    //q
    vesa_font[('q'*10)+0]=0x00;  //........ 0x00
    vesa_font[('q'*10)+1]=0x00;  //........ 0x00
    vesa_font[('q'*10)+2]=0x00;  //........ 0x00
    vesa_font[('q'*10)+3]=0x3E;  //..00000. 0x3E
    vesa_font[('q'*10)+4]=0x7E;  //.000000. 0x7E
    vesa_font[('q'*10)+5]=0x66;  //.00..00. 0x66
    vesa_font[('q'*10)+6]=0x7E;  //.000000. 0x7E
    vesa_font[('q'*10)+7]=0x3E;  //..00000. 0x3E
    vesa_font[('q'*10)+8]=0x06;  //.....00. 0x06
    vesa_font[('q'*10)+9]=0x06;  //.....00. 0x06

    //r
    vesa_font[('r'*10)+0]=0x00;  //........ 0x00
    vesa_font[('r'*10)+1]=0x00;  //........ 0x00
    vesa_font[('r'*10)+2]=0x00;  //........ 0x00
    vesa_font[('r'*10)+3]=0x6C;  //.00.00.. 0x6C
    vesa_font[('r'*10)+4]=0x7E;  //.000000. 0x7E
    vesa_font[('r'*10)+5]=0x70;  //.000.... 0x70
    vesa_font[('r'*10)+6]=0x60;  //.00..... 0x60
    vesa_font[('r'*10)+7]=0x60;  //.00..... 0x60
    vesa_font[('r'*10)+8]=0x60;  //.00..... 0x60
    vesa_font[('r'*10)+9]=0x00;  //........ 0x00

    //s
    vesa_font[('s'*10)+0]=0x00;  //........ 0x00
    vesa_font[('s'*10)+1]=0x00;  //........ 0x00
    vesa_font[('s'*10)+2]=0x3C;  //..0000.. 0x3C
    vesa_font[('s'*10)+3]=0x7E;  //.000000. 0x7E
    vesa_font[('s'*10)+4]=0x60;  //.00..... 0x60
    vesa_font[('s'*10)+5]=0x7E;  //.000000. 0x7E
    vesa_font[('s'*10)+6]=0x06;  //.....00. 0x06
    vesa_font[('s'*10)+7]=0x7E;  //.000000. 0x7E
    vesa_font[('s'*10)+8]=0x3C;  //..0000.. 0x3C
    vesa_font[('s'*10)+9]=0x00;  //........ 0x00

    //t
    vesa_font[('t'*10)+0]=0x00;  //........ 0x00
    vesa_font[('t'*10)+1]=0x18;  //...00... 0x18
    vesa_font[('t'*10)+2]=0x18;  //...00... 0x18
    vesa_font[('t'*10)+3]=0x7E;  //.000000. 0x7E
    vesa_font[('t'*10)+4]=0x7E;  //.000000. 0x7E
    vesa_font[('t'*10)+5]=0x18;  //...00... 0x18
    vesa_font[('t'*10)+6]=0x18;  //...00... 0x18
    vesa_font[('t'*10)+7]=0x18;  //...00... 0x18
    vesa_font[('t'*10)+8]=0x18;  //...00... 0x18
    vesa_font[('t'*10)+9]=0x00;  //........ 0x00

    //u
    vesa_font[('u'*10)+0]=0x00;  //........ 0x00
    vesa_font[('u'*10)+1]=0x00;  //........ 0x00
    vesa_font[('u'*10)+2]=0x00;  //........ 0x00
    vesa_font[('u'*10)+3]=0x66;  //.00..00. 0x66
    vesa_font[('u'*10)+4]=0x66;  //.00..00. 0x66
    vesa_font[('u'*10)+5]=0x66;  //.00..00. 0x66
    vesa_font[('u'*10)+6]=0x66;  //.00..00. 0x66
    vesa_font[('u'*10)+7]=0x7E;  //.000000. 0x7E
    vesa_font[('u'*10)+8]=0x3C;  //..0000.. 0x3C
    vesa_font[('u'*10)+9]=0x00;  //........ 0x00

    //v
    vesa_font[('v'*10)+0]=0x00;  //........ 0x00
    vesa_font[('v'*10)+1]=0x00;  //........ 0x00
    vesa_font[('v'*10)+2]=0x00;  //........ 0x00
    vesa_font[('v'*10)+3]=0x66;  //.00..00. 0x66
    vesa_font[('v'*10)+4]=0x66;  //.00..00. 0x66
    vesa_font[('v'*10)+5]=0x66;  //.00..00. 0x66
    vesa_font[('v'*10)+6]=0x66;  //.00..00. 0x66
    vesa_font[('v'*10)+7]=0x3C;  //..0000.. 0x3C
    vesa_font[('v'*10)+8]=0x18;  //...00... 0x18
    vesa_font[('v'*10)+9]=0x00;  //........ 0x00

    //w
    vesa_font[('w'*10)+0]=0x00;  //........ 0x00
    vesa_font[('w'*10)+1]=0x00;  //........ 0x00
    vesa_font[('w'*10)+2]=0x00;  //........ 0x00
    vesa_font[('w'*10)+3]=0xDB;  //00.00.00 0xDB
    vesa_font[('w'*10)+4]=0xDB;  //00.00.00 0xDB
    vesa_font[('w'*10)+5]=0xDB;  //00.00.00 0xDB
    vesa_font[('w'*10)+6]=0xDB;  //00.00.00 0xDB
    vesa_font[('w'*10)+7]=0xFF;  //00000000 0xFF
    vesa_font[('w'*10)+8]=0x66;  //.00..00. 0x66
    vesa_font[('w'*10)+9]=0x00;  //........ 0x00

    //x
    vesa_font[('x'*10)+0]=0x00;  //........ 0x00
    vesa_font[('x'*10)+1]=0x00;  //........ 0x00
    vesa_font[('x'*10)+2]=0x00;  //........ 0x00
    vesa_font[('x'*10)+3]=0x66;  //.00..00. 0x66
    vesa_font[('x'*10)+4]=0x66;  //.00..00. 0x66
    vesa_font[('x'*10)+5]=0x3C;  //..0000.. 0x3C
    vesa_font[('x'*10)+6]=0x3C;  //..0000.. 0x3C
    vesa_font[('x'*10)+7]=0x66;  //.00..00. 0x66
    vesa_font[('x'*10)+8]=0x66;  //.00..00. 0x66
    vesa_font[('x'*10)+9]=0x00;  //........ 0x00

    //y
    vesa_font[('y'*10)+0]=0x00;  //........ 0x00
    vesa_font[('y'*10)+1]=0x00;  //........ 0x00
    vesa_font[('y'*10)+2]=0x00;  //........ 0x00
    vesa_font[('y'*10)+3]=0x66;  //.00..00. 0x66
    vesa_font[('y'*10)+4]=0x66;  //.00..00. 0x66
    vesa_font[('y'*10)+5]=0x3E;  //..00000. 0x3E
    vesa_font[('y'*10)+6]=0x1C;  //...000.. 0x1C
    vesa_font[('y'*10)+7]=0x38;  //..000... 0x38
    vesa_font[('y'*10)+8]=0x70;  //.000.... 0x70
    vesa_font[('y'*10)+9]=0x00;  //........ 0x00

    //z
    vesa_font[('z'*10)+0]=0x00;  //........ 0x00
    vesa_font[('z'*10)+1]=0x00;  //........ 0x00
    vesa_font[('z'*10)+2]=0x00;  //........ 0x00
    vesa_font[('z'*10)+3]=0x7E;  //.000000. 0x7E
    vesa_font[('z'*10)+4]=0x7E;  //.000000. 0x7E
    vesa_font[('z'*10)+5]=0x1C;  //...000.. 0x1C
    vesa_font[('z'*10)+6]=0x38;  //..000... 0x38
    vesa_font[('z'*10)+7]=0x7E;  //.000000. 0x7E
    vesa_font[('z'*10)+8]=0x7E;  //.000000. 0x7E
    vesa_font[('z'*10)+9]=0x00;  //........ 0x00

    //0
    vesa_font[('0'*10)+0]=0x00;  //........ 0x00
    vesa_font[('0'*10)+1]=0x18;  //...00... 0x18
    vesa_font[('0'*10)+2]=0x3C;  //..0000.. 0x3C
    vesa_font[('0'*10)+3]=0x7E;  //.000000. 0x7E
    vesa_font[('0'*10)+4]=0x66;  //.00..00. 0x66
    vesa_font[('0'*10)+5]=0x66;  //.00..00. 0x66
    vesa_font[('0'*10)+6]=0x7E;  //.000000. 0x7E
    vesa_font[('0'*10)+7]=0x3C;  //..0000.. 0x3C
    vesa_font[('0'*10)+8]=0x18;  //...00... 0x18
    vesa_font[('0'*10)+9]=0x00;  //........ 0x00

    //1
    vesa_font[('1'*10)+0]=0x00;  //........ 0x00
    vesa_font[('1'*10)+1]=0x06;  //.....00. 0x06
    vesa_font[('1'*10)+2]=0x0E;  //....000. 0x0E
    vesa_font[('1'*10)+3]=0x1E;  //...0000. 0x1E
    vesa_font[('1'*10)+4]=0x36;  //..00.00. 0x36
    vesa_font[('1'*10)+5]=0x26;  //..0..00. 0x26
    vesa_font[('1'*10)+6]=0x06;  //.....00. 0x06
    vesa_font[('1'*10)+7]=0x06;  //.....00. 0x06
    vesa_font[('1'*10)+8]=0x06;  //.....00. 0x06
    vesa_font[('1'*10)+9]=0x00;  //........ 0x00

    //2
    vesa_font[('2'*10)+0]=0x00;  //........ 0x00
    vesa_font[('2'*10)+1]=0x3C;  //..0000.. 0x3C
    vesa_font[('2'*10)+2]=0x7E;  //.000000. 0x7E
    vesa_font[('2'*10)+3]=0x66;  //.00..00. 0x66
    vesa_font[('2'*10)+4]=0x0C;  //....00.. 0x0C
    vesa_font[('2'*10)+5]=0x18;  //...00... 0x18
    vesa_font[('2'*10)+6]=0x30;  //..00.... 0x30
    vesa_font[('2'*10)+7]=0x7E;  //.000000. 0x7E
    vesa_font[('2'*10)+8]=0x7E;  //.000000. 0x7E
    vesa_font[('2'*10)+9]=0x00;  //........ 0x00

    //3
    vesa_font[('3'*10)+0]=0x00;  //........ 0x00
    vesa_font[('3'*10)+1]=0x3C;  //..0000.. 0x3C
    vesa_font[('3'*10)+2]=0x7E;  //.000000. 0x7E
    vesa_font[('3'*10)+3]=0x06;  //.....00. 0x06
    vesa_font[('3'*10)+4]=0x1C;  //...000.. 0x1C
    vesa_font[('3'*10)+5]=0x1C;  //...000.. 0x1C
    vesa_font[('3'*10)+6]=0x06;  //.....00. 0x06
    vesa_font[('3'*10)+7]=0x7E;  //.000000. 0x7E
    vesa_font[('3'*10)+8]=0x3C;  //..0000.. 0x3C
    vesa_font[('3'*10)+9]=0x00;  //........ 0x00

    //4
    vesa_font[('4'*10)+0]=0x00;  //........ 0x00
    vesa_font[('4'*10)+1]=0x0E;  //....000. 0x0E
    vesa_font[('4'*10)+2]=0x1C;  //...000.. 0x1C
    vesa_font[('4'*10)+3]=0x38;  //..000... 0x38
    vesa_font[('4'*10)+4]=0x76;  //.000.00. 0x76
    vesa_font[('4'*10)+5]=0x7E;  //.000000. 0x7E
    vesa_font[('4'*10)+6]=0x7E;  //.000000. 0x7E
    vesa_font[('4'*10)+7]=0x06;  //....00.. 0x06
    vesa_font[('4'*10)+8]=0x06;  //....00.. 0x06
    vesa_font[('4'*10)+9]=0x00;  //........ 0x00

    //5
    vesa_font[('5'*10)+0]=0x00;  //........ 0x00
    vesa_font[('5'*10)+1]=0x7E;  //.000000. 0x7E
    vesa_font[('5'*10)+2]=0x7E;  //.000000. 0x7E
    vesa_font[('5'*10)+3]=0x60;  //.00..... 0x60
    vesa_font[('5'*10)+4]=0x7E;  //.000000. 0x7E
    vesa_font[('5'*10)+5]=0x7E;  //.000000. 0x7E
    vesa_font[('5'*10)+6]=0x06;  //.....00. 0x06
    vesa_font[('5'*10)+7]=0x7E;  //.000000. 0x7E
    vesa_font[('5'*10)+8]=0x3C;  //..0000.. 0x3C
    vesa_font[('5'*10)+9]=0x00;  //........ 0x00

    //6
    vesa_font[('6'*10)+0]=0x00;  //........ 0x00
    vesa_font[('6'*10)+1]=0x3C;  //..0000.. 0x3C
    vesa_font[('6'*10)+2]=0x7E;  //.000000. 0x7E
    vesa_font[('6'*10)+3]=0x60;  //.00..... 0x60
    vesa_font[('6'*10)+4]=0x6C;  //.00.00.. 0x6C
    vesa_font[('6'*10)+5]=0x7E;  //.000000. 0x7E
    vesa_font[('6'*10)+6]=0x66;  //.00..00. 0x66
    vesa_font[('6'*10)+7]=0x7E;  //.000000. 0x7E
    vesa_font[('6'*10)+8]=0x3C;  //..0000.. 0x3C
    vesa_font[('6'*10)+9]=0x00;  //........ 0x00

    //7
    vesa_font[('7'*10)+0]=0x00;  //........ 0x00
    vesa_font[('7'*10)+1]=0x7E;  //.000000. 0x7E
    vesa_font[('7'*10)+2]=0x7E;  //.000000. 0x7E
    vesa_font[('7'*10)+3]=0x0E;  //....000. 0x0E
    vesa_font[('7'*10)+4]=0x1C;  //...000.. 0x1C
    vesa_font[('7'*10)+5]=0x1C;  //...000.. 0x1C
    vesa_font[('7'*10)+6]=0x38;  //..000... 0x38
    vesa_font[('7'*10)+7]=0x38;  //..000... 0x38
    vesa_font[('7'*10)+8]=0x30;  //..00.... 0x30
    vesa_font[('7'*10)+9]=0x00;  //........ 0x00

    //8
    vesa_font[('8'*10)+0]=0x00;  //........ 0x00
    vesa_font[('8'*10)+1]=0x3C;  //..0000.. 0x3C
    vesa_font[('8'*10)+2]=0x7E;  //.000000. 0x7E
    vesa_font[('8'*10)+3]=0x66;  //.00..00. 0x66
    vesa_font[('8'*10)+4]=0x7E;  //.000000. 0x7E
    vesa_font[('8'*10)+5]=0x3C;  //..0000.. 0x3C
    vesa_font[('8'*10)+6]=0x66;  //.00..00. 0x66
    vesa_font[('8'*10)+7]=0x7E;  //.000000. 0x7E
    vesa_font[('8'*10)+8]=0x3C;  //..0000.. 0x3C
    vesa_font[('8'*10)+9]=0x00;  //........ 0x00

    //9
    vesa_font[('9'*10)+0]=0x00;  //........ 0x00
    vesa_font[('9'*10)+1]=0x3C;  //..0000.. 0x3C
    vesa_font[('9'*10)+2]=0x7E;  //.000000. 0x7E
    vesa_font[('9'*10)+3]=0x66;  //.00..00. 0x66
    vesa_font[('9'*10)+4]=0x7E;  //.000000. 0x7E
    vesa_font[('9'*10)+5]=0x3E;  //..00000. 0x3E
    vesa_font[('9'*10)+6]=0x06;  //.....00. 0x06
    vesa_font[('9'*10)+7]=0x7E;  //.000000. 0x7E
    vesa_font[('9'*10)+8]=0x3C;  //..0000.. 0x3C
    vesa_font[('9'*10)+9]=0x00;  //........ 0x00

    //.
    vesa_font[('.'*10)+0]=0x00;  //........ 0x00
    vesa_font[('.'*10)+1]=0x00;  //........ 0x00 
    vesa_font[('.'*10)+2]=0x00;  //........ 0x00
    vesa_font[('.'*10)+3]=0x00;  //........ 0x00
    vesa_font[('.'*10)+4]=0x00;  //........ 0x00
    vesa_font[('.'*10)+5]=0x00;  //........ 0x00
    vesa_font[('.'*10)+6]=0x00;  //........ 0x00
    vesa_font[('.'*10)+7]=0x18;  //...00... 0x18
    vesa_font[('.'*10)+8]=0x18;  //...00... 0x18
    vesa_font[('.'*10)+9]=0x00;  //........ 0x00

    //:
    vesa_font[(':'*10)+0]=0x00;  //........ 0x00
    vesa_font[(':'*10)+1]=0x00;  //........ 0x00 
    vesa_font[(':'*10)+2]=0x00;  //........ 0x00
    vesa_font[(':'*10)+3]=0x18;  //...00... 0x18
    vesa_font[(':'*10)+4]=0x18;  //...00... 0x18
    vesa_font[(':'*10)+5]=0x00;  //........ 0x00
    vesa_font[(':'*10)+6]=0x00;  //........ 0x00
    vesa_font[(':'*10)+7]=0x18;  //...00... 0x18
    vesa_font[(':'*10)+8]=0x18;  //...00... 0x18
    vesa_font[(':'*10)+9]=0x00;  //........ 0x00

    //!
    vesa_font[('!'*10)+0]=0x00;  //........ 0x00
    vesa_font[('!'*10)+1]=0x18;  //...00... 0x18
    vesa_font[('!'*10)+2]=0x18;  //...00... 0x18
    vesa_font[('!'*10)+3]=0x18;  //...00... 0x18
    vesa_font[('!'*10)+4]=0x18;  //...00... 0x18
    vesa_font[('!'*10)+5]=0x18;  //...00... 0x18
    vesa_font[('!'*10)+6]=0x00;  //........ 0x00
    vesa_font[('!'*10)+7]=0x18;  //...00... 0x18
    vesa_font[('!'*10)+8]=0x18;  //...00... 0x18
    vesa_font[('!'*10)+9]=0x00;  //........ 0x00

    //?
    vesa_font[('?'*10)+0]=0x00;  //........ 0x00
    vesa_font[('?'*10)+1]=0x3C;  //..0000.. 0x3C
    vesa_font[('?'*10)+2]=0x66;  //.00..00. 0x66
    vesa_font[('?'*10)+3]=0x66;  //.00..00. 0x66
    vesa_font[('?'*10)+4]=0x0C;  //....00.. 0x0C
    vesa_font[('?'*10)+5]=0x18;  //...00... 0x18
    vesa_font[('?'*10)+6]=0x18;  //...00... 0x18
    vesa_font[('?'*10)+7]=0x00;  //........ 0x00
    vesa_font[('?'*10)+8]=0x18;  //...00... 0x18
    vesa_font[('?'*10)+9]=0x00;  //........ 0x00

    //-
    vesa_font[('-'*10)+0]=0x00;  //........ 0x00
    vesa_font[('-'*10)+1]=0x00;  //........ 0x00
    vesa_font[('-'*10)+2]=0x00;  //........ 0x00
    vesa_font[('-'*10)+3]=0x00;  //........ 0x00
    vesa_font[('-'*10)+4]=0x3C;  //..0000.. 0x3C
    vesa_font[('-'*10)+5]=0x3C;  //..0000.. 0x3C
    vesa_font[('-'*10)+6]=0x00;  //........ 0x00
    vesa_font[('-'*10)+7]=0x00;  //........ 0x00
    vesa_font[('-'*10)+8]=0x00;  //........ 0x00
    vesa_font[('-'*10)+9]=0x00;  //........ 0x00

    //_
    vesa_font[('_'*10)+0]=0x00;  //........ 0x00
    vesa_font[('_'*10)+1]=0x00;  //........ 0x00
    vesa_font[('_'*10)+2]=0x00;  //........ 0x00
    vesa_font[('_'*10)+3]=0x00;  //........ 0x00
    vesa_font[('_'*10)+4]=0x00;  //........ 0x00
    vesa_font[('_'*10)+5]=0x00;  //........ 0x00
    vesa_font[('_'*10)+6]=0x00;  //........ 0x00
    vesa_font[('_'*10)+7]=0x00;  //........ 0x00
    vesa_font[('_'*10)+8]=0xFF;  //00000000 0xFF
    vesa_font[('_'*10)+9]=0xFF;  //00000000 0xFF

    //=
    vesa_font[('='*10)+0]=0x00;  //........ 0x00
    vesa_font[('='*10)+1]=0x00;  //........ 0x00
    vesa_font[('='*10)+2]=0x00;  //........ 0x00
    vesa_font[('='*10)+3]=0x7E;  //.000000. 0x7E
    vesa_font[('='*10)+4]=0x7E;  //.000000. 0x7E
    vesa_font[('='*10)+5]=0x00;  //........ 0x00
    vesa_font[('='*10)+6]=0x7E;  //.000000. 0x7E
    vesa_font[('='*10)+7]=0x7E;  //.000000. 0x7E
    vesa_font[('='*10)+8]=0x00;  //........ 0x00
    vesa_font[('='*10)+9]=0x00;  //........ 0x00

    //+
    vesa_font[('+'*10)+0]=0x00;  //........ 0x00
    vesa_font[('+'*10)+1]=0x00;  //........ 0x00
    vesa_font[('+'*10)+2]=0x00;  //........ 0x00
    vesa_font[('+'*10)+3]=0x18;  //...00... 0x18
    vesa_font[('+'*10)+4]=0x18;  //...00... 0x18
    vesa_font[('+'*10)+5]=0x7E;  //.000000. 0x7E
    vesa_font[('+'*10)+6]=0x7E;  //.000000. 0x7E
    vesa_font[('+'*10)+7]=0x18;  //...00... 0x18
    vesa_font[('+'*10)+8]=0x18;  //...00... 0x18
    vesa_font[('+'*10)+9]=0x00;  //........ 0x00

    //space
    vesa_font[(' '*10)+0]=0x00;  //........ 0x00
    vesa_font[(' '*10)+1]=0x00;  //........ 0x00
    vesa_font[(' '*10)+2]=0x00;  //........ 0x00
    vesa_font[(' '*10)+3]=0x00;  //........ 0x00
    vesa_font[(' '*10)+4]=0x00;  //........ 0x00
    vesa_font[(' '*10)+5]=0x00;  //........ 0x00
    vesa_font[(' '*10)+6]=0x00;  //........ 0x00
    vesa_font[(' '*10)+7]=0x00;  //........ 0x00
    vesa_font[(' '*10)+8]=0x00;  //........ 0x00
    vesa_font[(' '*10)+9]=0x00;  //........ 0x00
}

//Scan functions
void scan_monitor(uint16_t line, uint16_t column, uint16_t lenght, uint16_t height, uint32_t memory_position) {
    uint16_t *memory = (uint16_t *) (memory_position);
	uint32_t offset=0;

	for(uint32_t i=0; i<height; i++) {
		for(uint32_t j=0; j<lenght; j++) {
			memory[offset]=read_pixel( (line+i), (column+j) );
			offset++;
		}
	}
}

void draw_monitor(uint16_t line, uint16_t column, uint16_t lenght, uint16_t height, uint32_t memory_position) {
	uint16_t *memory = (uint16_t *) (memory_position);
	uint32_t offset=0;

	for(uint32_t i=0; i<height; i++) {
		for(uint32_t j=0; j<lenght; j++) {
			pixel( (line+i), (column+j), memory[offset]);
			offset++;
		}
	}
}

//Mouse
void draw_mouse(uint32_t x, uint32_t y) {
    for(uint32_t i=0; i<10; i++) {
        pixel(x, y, BLACK);
        x++;    //next line
    }
}

//special
void p(uint8_t *message) {
    if(print_line==60) {
        draw_square(0, 0, 800, 600, WHITE);
        print_line=0;
    }

    print(message, print_line, 0, BLACK);

    print_line++;
}

void pv(uint64_t value) {
    if(print_line==60) {
        draw_square(0, 0, 800, 600, WHITE);
        print_line=0;
    }

    print_var(value, print_line, 0, BLACK);

    print_line++;
}

void ph(uint32_t value) {
    if(print_line==60) {
        draw_square(0, 0, 800, 600, WHITE);
        print_line=0;
    }

    print_hex(value, print_line, 0, BLACK);

    print_line++;
}
