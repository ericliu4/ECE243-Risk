#include <stdio.h>

#define RLEDs ((volatile long *) 0xFF200000)

volatile int * PRIVATE_TIMER_PTR = (int *)0xFFFEC600;


void countDown(){
	int status;
	*PRIVATE_TIMER_PTR = 100000000;
	*(PRIVATE_TIMER_PTR +2) = 0b011;

	while(*(PRIVATE_TIMER_PTR +3) == 0){
		status = *(PRIVATE_TIMER_PTR +3);
	}
	*(PRIVATE_TIMER_PTR +3) = 0;
}

int main() {
	unsigned char byte1 = 0;
	unsigned char byte2 = 0;
	unsigned char byte3 = 0;
	
  	volatile int * PS2_ptr = (int *) 0xFF200108;  // PS/2 port address

	int PS2_data, RVALID;

	while (1) {
		countDown();
		PS2_data = *(PS2_ptr);	// read the Data register in the PS/2 port
		RVALID = (PS2_data & 0x8000);	// extract the RVALID field
		if (RVALID != 0)
		{
			/* always save the last three bytes received */
			byte1 = byte2;
			byte2 = byte3;
			byte3 = PS2_data & 0xFF;
		}
		if ( (byte2 == 0xAA) && (byte3 == 0x00) )
		{
			// mouse inserted; initialize sending of data
			*(PS2_ptr) = 0xF4;
		}
		// Display last byte on Red LEDs
		*RLEDs = byte3;
        printf("RValid: %d, byte1: %d, byte2: %d, byte3 %d \n", RVALID, byte1, byte2, byte3);
	}
}