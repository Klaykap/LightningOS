org 0x7C00

bits 16

start:
  jmp main
  TIMES 3-($-$$) DB 0x90

  ;Fake BPB for BIOS
  db "mkfs.fat"
  dw 512
  db 1
  dw 1
  db 2
  dw 224
  dw 2880
  db 0xF0
  dw 9
  dw 72
  dw 2
  dd 0
  dd 0
  db 0
  db 0
  db 0x29
  dd 0x2D7E5A1A
  db "NO NAME    "
  db "FAT12   "

main:
  xor ax, ax
  mov ds, ax
  mov ss, ax
  mov sp, 0x7C00

  ;load VESA info block
  mov ax, 0  ;segment 0
  mov es, ax
  mov di, 0x1000  ;offset 1000
  mov ax, 0x4F01  ;get VBE mode information block
  mov cx, 0x4114  ;graphic mode 800x600 16 bpp
  int 0x10  ;load informations

  ;set VESA video mode
  mov ax, 0x4F02  ;set VBE graphic mode
  mov bx, 0x4114  ;graphic mode 800x600 16bpp
  int 0x10  ;start BIOS interrupt

  ;loading 104 sectors from usb/cdrom

  ;read usb first head
  mov ax, 0x1000  ;load kernel to 10000h
  mov es, ax  ;load kernel to 10000h
  mov bx, 0  ;offset 0 
  mov ah, 0x02  ;read function
  mov al, 18  ;one head
  mov ch, 0  ;cylinder 0
  mov dh, 0  ;head 0
  mov cl, 2  ;start sector is 2
  mov dl, 0x00  ;usb
  int 13h  ;read

  ;read usb second head
  mov ax, 0x1000  ;load kernel to 10000h
  mov es, ax  ;load kernel to 10000h
  mov bx, 9216  ;offset 9216
  mov ah, 0x02  ;read function
  mov al, 18  ;one head
  mov ch, 0  ;cylinder 0
  mov dh, 1  ;head 1
  mov cl, 2  ;sector 2
  mov dl, 0x00  ;usb
  int 13h  ;read

  ;read usb third head
  mov ax, 0x1000  ;load kernel to 10000h
  mov es, ax  ;load kernel to 10000h
  mov bx, 18432  ;offset 18432
  mov ah, 0x02  ;read function
  mov al, 18  ;one head
  mov ch, 1  ;cylinder 1
  mov dh, 0  ;head 0
  mov cl, 2  ;sector 2
  mov dl, 0x00  ;usb
  int 13h  ;read

  ;read usb fourth head
  mov ax, 0x1000  ;load kernel to 10000h
  mov es, ax  ;load kernel to 10000h
  mov bx, 27648  ;offset 27648
  mov ah, 0x02  ;read function
  mov al, 50  ;one head
  mov ch, 1  ;cylinder 1
  mov dh, 1  ;head 1
  mov cl, 2  ;sector 2
  mov dl, 0x00  ;usb
  int 13h  ;read

  jmp load_gdt 

  ;global descriptor table
  gdt:

  gdt_null:
  dq 0

  gdt_code:
  dw 0FFFFh
  dw 0

  db 0
  db 10011010b
  db 11001111b
  db 0

  gdt_data:
  dw 0xFFFF
  dw 0

  db 0
  db 10010010b
  db 11001111b
  db 0

  gdt_end:

  gdt_desc:
   dw gdt_end - gdt - 1
   dd gdt

  ;load gdt
  load_gdt:
    cli
    lgdt [gdt_desc]

  ;jump into protected mode
  mov eax, cr0
  or eax, 1
  mov cr0, eax
  jmp 0x0008:clear_pipe

  ;NOW WE ARE IN PROTECTED MODE
  bits 32

  clear_pipe:
  mov ax, 0x0010
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ss, ax

  mov esp, 0x9FFFF  ;set stack pointer

  ;JUMP TO EXECUTE LIGHTNINGOS!
  jmp 0x10000

times 510-($-$$) db 0
dw 0xAA55
