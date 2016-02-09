/*
 * A class to represent an image and calculate which
 * diods should light up given the current position.
 */
#include "ImageHandler.h"
#include "OutputInterpreter.h"

#define TIFO_LEN 32

static int img[TIFO_LEN] = {0x00, 0x80, 0x80, 0xff, 0x80, 0x80, 0x00,  //T
		0x00, 0x81, 0xff, 0x81, 0x00, 								   //I
		0x00, 0xff, 0x88, 0x88, 0x88, 0x88, 0x80, 0x00,				   //F
		0x00, 0x7e, 0xc3, 0x81, 0x81, 0xc3, 0x7e, 0x00, 			   //O
		0x00, 0xfb, 0xfb, 0x00};									   //!
static int lastIndex = 0;

void updateImage(float pos) {
	int currIndex = pos*TIFO_LEN;
	if (currIndex < 0) {
		currIndex = 0;
	} else if (currIndex > TIFO_LEN) {
		currIndex = TIFO_LEN - 1;
	}
	if (currIndex != lastIndex) {
		lastIndex = currIndex;
		updateOutputs(img[currIndex]);
	}
}
