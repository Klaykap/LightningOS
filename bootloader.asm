org 0x7c00
bits 16

start:
  xor ax, ax
  mov ds, ax
  mov ss, ax
  mov sp, 0x7c00

  ;set VESA graphic mode
  mov ax, 0x0  ;segment 0
  mov es, ax
  mov di, 0x1000  ;offset 0
  mov ax, 0x4F01  ;get VBE mode information block
  mov cx, 0x4114  ;get graphic mode 800x600 16 bpp
  int 0x10  ;load informations

  mov ax, 0x4F02  ;set VBE graphic mode
  mov bx, 0x4114  ;set graphic mode 800x600 16bpp
  int 0x10  ;start BIOS interrupt

  ;LOAD KERNEL
  ;reset floppy
  mov ah, 0
  mov dl, 0
  int 13h  ;call reset

  ;read floppy first 72 sectors
  mov ax, 0x1000  ;load kernel to 1000h
  mov es, ax  ;load kernel to 1000h
  mov bx, 0  ;offset 0 

  mov ah, 0x02  ;read function
    mov al, 72  ;72 sectors
    mov ch, 0  ;track 0
    mov cl, 2  ;start sector is 2
    mov dh, 0  ;head 0
    mov dl, 80h  ;hard disc

  int 13h  ;call read

  ;read floppy second 72 sectors
  mov ax, 0x1000  ;load kernel to 1000h
  mov es, ax  ;load kernel to 1000h
  mov bx, 36864  ;offset 36864 

  mov ah, 0x02  ;read function
    mov al, 72  ;72 sectors
    mov ch, 0  ;track 0
    mov cl, 74  ;start sector is 74
    mov dh, 0  ;head 0
    mov dl, 80h  ;hard disc

  int 13h  ;call read

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
  dw 0FFFFh
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

  mov esp, 9FFFFh  ;set stack pointer

  ;JUMP TO EXECUTE KERNEL!
  jmp 0x10000

times 510-($-$$) db 0 ;Pad remainder of boot sector with 0s
dw 0xAA55  ;The standard PC boot signature
