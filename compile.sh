#!/bin/bash
nasm -f bin source/bootloader/bootloader.asm -o source/bootloader/bootloader.bin #bootloader

nasm -f elf32 source/lightningos.asm -o source/compile/lightningos_asm.bin #assembler part

gcc -m32 -c -fno-stack-protector -std=gnu99 -ffreestanding -fno-PIC source/lightningos.c -o source/compile/lightningos_c.bin

ld -m elf_i386 --oformat=binary -T source/compile/link.ld -o source/compile/lightningos.bin source/compile/lightningos_asm.bin source/compile/lightningos_c.bin

dd if=source/bootloader/bootloader.bin of=lightningos.img conv=notrunc seek=0
dd if=source/compile/lightningos.bin of=lightningos.img conv=notrunc seek=1

qemu-system-i386 -soundhw pcspk -fda lightningos.img

sleep 50
