//LIGHTNINGOS

#define UHCI_COMMAND 0x00
#define UHCI_STATUS 0x02
#define UHCI_INTERRUPT 0x04
#define UHCI_FRAME_NUMBER 0x06
#define UHCI_FRAME_LIST_ADDRESS 0x08
#define UHCI_START_OF_FRAME 0x0C
#define UHCI_SC1 0x10
#define UHCI_SC2 0x12

#define USB 0

void uhci_reset(uint32_t usb) {
	uint16_t sc1=0;
	uint16_t sc2=0;

	sc1 = inw(uhci_base[USB] + UHCI_SC1);
	sc2 = inw(uhci_base[USB] + UHCI_SC2);

	wait();

    outw(uhci_base[usb] + UHCI_SC1, (sc1 | 0x204));
    outw(uhci_base[usb] + UHCI_SC2, (sc2 | 0x204));
}

void uhci(void) {
	uhci_reset(USB);
}
