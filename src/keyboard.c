#include "keyboard.h"

volatile int * PS2_ptr = (int *)PS2_BASE;
char byte1 = 0, byte2 = 0, byte3 = 0;
unsigned char keyPressed;

/*******************************************************************************
* This program demonstrates use of the PS/2 port by displaying the last three
* bytes of data received from the PS/2 port on the HEX displays.
******************************************************************************/
int testerMain(void) {
	/* Declare volatile pointers to I/O registers (volatile means that IO load
	and store instructions will be used to access these pointer locations,
	instead of regular memory loads and stores) */


	keyboardSetup();
	while (1) {
		for(int i = 0; i < 10000; ++i);
		pollKeyboard();
		HEX_PS2(byte1, byte2, byte3); //for debugging
	}

}

void keyboardSetup(){
	// PS/2 mouse needs to be reset (must be already plugged in)
	*(PS2_ptr) = 0xFF; // reset
}

void pollKeyboard(){
	int PS2_data, RVALID;
	PS2_data = *(PS2_ptr); // read the Data register in the PS/2 port
	RVALID = PS2_data & 0x8000; // extract the RVALID field
	if (RVALID) { //if there are some value:
	/* shift the next data byte into the display */
	byte1 = byte2;
	byte2 = byte3;
	byte3 = PS2_data & 0xFF;  //take the first 8 bits (2 HEX) 

	//need to read posedge
	if(byte2 == (char)0xF0 || byte2 == (char)0xE0){
		if(keyPressed){
			return;
		}
		//read is probably good:
			switch(byte3){
				case (char)0x1D: //W key
				case (char)0x75: //Up arrow (numpad or regular)
					//no need to check only for negedge
					if(cursor.yPos > 2)
						cursor.yPos -= 2;
					break;
				case (char)0x1C: //A key
				case (char)0x6B: //left arrow
					if(cursor.xPos > 2)
						cursor.xPos -= 2;
					break;
				case (char)0x1B: //S key
				case (char)0x74: //down arrow
				if(cursor.yPos < SCREEN_HEIGHT -2)
					cursor.yPos += 2;
					break;
				case (char)0x23: //D key
				case (char)0x72: //right arrow
					if(cursor.xPos < SCREEN_WIDTH -2)
						cursor.xPos += 2;
					break;
				case (char)0x29:
				case (char)0x5A:
					cursor.clicked = true;
					keyPressed = true;
					break;
			}

	} 
	keyPressed = false;	   	   	   

	if ((byte2 == (char)0xAA) && (byte3 == (char)0x00))
		// mouse inserted; initialize sending of data
		*(PS2_ptr) = 0xF4;
	}
}


//for debugging - displays PS-2 packets on thig
void HEX_PS2(char b1, char b2, char b3) {
	volatile int * HEX3_HEX0_ptr = (int *)HEX3_HEX0_BASE;
	volatile int * HEX5_HEX4_ptr = (int *)HEX5_HEX4_BASE;
	/* SEVEN_SEGMENT_DECODE_TABLE gives the on/off settings for all segments in
	* a single 7-seg display in the DE1-SoC Computer, for the hex digits 0 - F
	*/
	unsigned char seven_seg_decode_table[] = {
	0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,
	0x7F, 0x67, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
	unsigned char hex_segs[] = {0, 0, 0, 0, 0, 0, 0, 0};
	unsigned int shift_buffer, nibble;
	unsigned char code;
	int i;
	shift_buffer = (b1 << 16) | (b2 << 8) | b3;
	for (i = 0; i < 6; ++i) {
	nibble = shift_buffer & 0x0000000F; // character is in rightmost nibble (last 4 bits, single digit to switch into HEX)
	code = seven_seg_decode_table[nibble];
	hex_segs[i] = code;
	shift_buffer = shift_buffer >> 4;
	}
	/* drive the hex displays */
	*(HEX3_HEX0_ptr) = *(int *)(hex_segs);
	*(HEX5_HEX4_ptr) = *(int *)(hex_segs + 4);
}


void clearFifo(){
	int PS2_data, RVALID;
	while(1){
		PS2_data = *(PS2_ptr); // read the Data register in the PS/2 port
		RVALID = PS2_data & 0x8000; // extract the RVALID field
		if (RVALID) { //if there are some value:
			continue;
		}
		return;
	}

}