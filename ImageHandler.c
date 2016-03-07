/*
 * A class to represent an image and calculate which
 * diods should light up given the current position.
 */

#include "ImageHandler.h"
#include "OutputInterpreter.h"

#define TIFO_LEN 44

//static unsigned int img[TIFO_LEN] = {0x0180, 0x0240, 0x0420, 0x0810, 0x1008, 0x2004, 0x4002, 0x8001};
//static unsigned int img[TIFO_LEN] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0x0000,0x0001,0x0002,0x0004,0x0008,0x0010,0x0020,0x0040,0x0080,0x0100,0x0200,0x0400,0x0800,0x1000,0x2000,0xffff};

static unsigned int img[TIFO_LEN] = {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
							0x0000, 0xc000, 0xc000, 0xffff, 0xc000, 0xc000, 0x0000,      	//T
                            0x0000, 0xc003, 0xffff, 0xc003, 0x0000,                  		//I
                            0x0000, 0xffff, 0xc0c0, 0xc0c0, 0xc0c0, 0xc0c0, 0xc000, 0x0000,	//F
                            0x0000, 0x3ffc, 0x6006, 0xc003, 0xc003, 0x6006, 0x3ffc, 0x0000,	//O
                            0x0000, 0xfff3, 0xfff3, 0x0000,                       			//!
							0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000};

static int lastIndex = 0;

void initImageHandler(){
	initOutputs();
}
/*
 * This function determines which part of the image or message to print out.
 * Also sends the determined column to the updateOutputs function.
 */
void updateImage(float pos) {
	// Compute which column to print
	int currIndex = pos*TIFO_LEN;

	// If index is out of range (lower), set index to first possible position
	if (currIndex < 0) {
		currIndex = 0;
	// If index is out of range (higher), set index to last possible position
	} else if (currIndex > TIFO_LEN) {
		currIndex = TIFO_LEN - 1;
	}
	// If current index differs from last index
	if (currIndex != lastIndex) {
		// Set current index to last index
		lastIndex = currIndex;
		// Send current column to output
		updateOutputs(img[currIndex]);
	}
}

void accelDrawer(int val) {
	int nbrOfLights = (((float)val)/32768)*8;
	int pixels = 0;
	int i = (nbrOfLights<0 ? -nbrOfLights : nbrOfLights);
	for(i=0; i>0;i--) {
		if (nbrOfLights<0) {
			pixels <<= 1;
			pixels |= 0x100;
		} else {
			pixels >>= 1;
			pixels |= 0x80;
		}
	}
	updateOutputs(val);
}
