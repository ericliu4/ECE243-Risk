#include "address_map_nios2.h"
#include "PS2.h"
#include <stdio.h>
#include "../src/structs.h"

/** GLOBALS **/
int count = 0;
mouse_movement movement;



/*
typedef struct {
    short dx;
    short dy;

    short xPos;
    short yPos;

    char left_pressed_bit;
    char right_pressed_bit;
}mouse_movement; */



/*******************************************************************************
 * This program demonstrates use of the PS/2 port by displaying the last three
 * bytes of data received from the PS/2 port on the HEX displays.
 ******************************************************************************/
int oldMain(void) {
    /* Declare volatile pointers to I/O registers (volatile means that IO load
       and store instructions will be used to access these pointer locations,
       instead of regular memory loads and stores) */
    volatile int * PS2_ptr = (int *)PS2_BASE;

    int  PS2_data, RVALID;
    char byte1 = 0, byte2 = 0, byte3 = 0;

    // PS/2 mouse needs to be reset (must be already plugged in)
    *(PS2_ptr) = 0xFF; // reset

    while (1) {
        PS2_data = *(PS2_ptr);        // read the Data register in the PS/2 port
        RVALID   = PS2_data & 0x8000; // extract the RVALID field
        if (RVALID) {
            /* shift the next data byte into the display */
            byte1 = byte2;
            byte2 = byte3;
            byte3 = PS2_data & 0xFF;
            if(byte1 & 0x08){ //checking for "always 1" bit
                HEX_PS2(byte1, byte2, byte3);
            } /* else{
                printf("Bytes not lined up!\n");
            }*/

            if ((byte2 == (char)0xAA) && (byte3 == (char)0x00))
                // mouse inserted; initialize sending of data
                *(PS2_ptr) = 0xF4;
        }
    }
}

/****************************************************************************************
 * Subroutine to show a string of HEX data on the HEX displays
****************************************************************************************/
void HEX_PS2(char b1, char b2, char b3) {
    ++count;
    //if(count != 15)
    //    return;
    volatile int * HEX3_HEX0_ptr = (int *)HEX3_HEX0_BASE;
    volatile int * HEX5_HEX4_ptr = (int *)HEX5_HEX4_BASE;

    /* SEVEN_SEGMENT_DECODE_TABLE gives the on/off settings for all segments in
     * a single 7-seg display in the DE1-SoC Computer, for the hex digits 0 - F
     */
    unsigned char seven_seg_decode_table[] = { //0->F for hex display
        0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,
        0x7F, 0x67, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
    unsigned char hex_segs[] = {0, 0, 0, 0, 0, 0, 0, 0};
    unsigned int  shift_buffer, nibble;
    unsigned char code;
    int           i;

    shift_buffer = (b1 << 16) | (b2 << 8) | b3;
    for (i = 0; i < 6; ++i) {
        nibble = shift_buffer & 0x0000000F; // character is in rightmost nibble
        code   = seven_seg_decode_table[nibble];
        hex_segs[i]  = code;
        shift_buffer = shift_buffer >> 4;
    }

        //Get dx/dy movement
    int x_sign_bit = b1 & 0x10; //0b00010000
    int y_sign_bit = (b1 & 0x20) >> 1; //0b00100000 //moves it to be a 1
    int left_pressed_bit = b1 & 0x01;// read 1st bit of the 1st byte of 3-byte movement packet
    int right_pressed_bit = b1 & 0x02; // read 2nd bit of the 1st byte of 3-byte movement packet

    

    //Constructing movement struct element
    movement.left_pressed_bit = left_pressed_bit;
    movement.right_pressed_bit = right_pressed_bit;

    

    if(x_sign_bit == 0) { //positive dx

    movement.dx = b2;

    } else { //negative dx with sign extension

    int sign_extended_twos =   0xFFFFFF00 + b2 ; // Equivalent to 0xFFFFFE00 + byte 2

    movement.dx = sign_extended_twos;

    }

    if(x_sign_bit == 0) { //positive dx
    movement.dx = b2;
    } else { //negative dx with sign extension
    int sign_extended_twos =   0xFFFFFF00 + b2 ; // Equivalent to 0xFFFFFE00 + byte 2
    movement.dx = sign_extended_twos;
    }

    if(y_sign_bit == 0) { //positive dx
    movement.dy = b3;
    } else { //negative dx
    int sign_extended_twos = 0xFFFFFF00 + b3;
    movement.dy = sign_extended_twos;
    }


    movement.dy = movement.dy >> 4;
    movement.dx = movement.dx >> 4;
   

    movement.xPos += (movement.dx);
    movement.yPos += (movement.dy);

    //return movement;

    



    /* drive the hex displays */
    *(HEX3_HEX0_ptr) = *(int *)(hex_segs);
    *(HEX5_HEX4_ptr) = *(int *)(hex_segs + 4);

    //DO NOT UNCOMMENT UNLESS YOU ARE SURE -- It slows down the program and breaks stuff
    if(count == 31){
        printf("Left Button: %d, Right Button: %d\n dX: %d, dY: %d\n xPos: %d, yPos: %d\n", movement.left_pressed_bit, movement.right_pressed_bit, movement.dx, movement.dy, movement.xPos, movement.yPos);
        count = 0;
    }
    
}


