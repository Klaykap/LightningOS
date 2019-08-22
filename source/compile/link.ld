/*
*  link.ld
*/
OUTPUT_FORMAT(bin)
ENTRY(start)
SECTIONS
 {
   . = 0x10000;
   .data : { *(.data) }
   .bss  : { *(.bss)  }
 }
