/*
 * A class to represent an image and calculate which
 * diods should light up given the current position.
 */

#include "ImageHandler.h"
#include "OutputInterpreter.h"

#define TIFO_LEN 32

static unsigned int img[TIFO_LEN] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0x0000,0x0001,0x0002,0x0004,0x0008,0x0010,0x0020,0x0040,0x0080,0x0100,0x0200,0x0400,0x0800,0x1000,0x2000,0xffff};
/*
static unsigned int img[TIFO_LEN] = {0x00, 0x80, 0x80, 0xff, 0x80, 0x80, 0x00,      //T
                            0x00, 0x81, 0xff, 0x81, 0x00,                  //I
                            0x00, 0xff, 0x88, 0x88, 0x88, 0x88, 0x80, 0x00,//F
                            0x00, 0x7e, 0xc3, 0x81, 0x81, 0xc3, 0x7e, 0x00,//O
                            0x00, 0xfb, 0xfb, 0x00};                       //!
*/
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
