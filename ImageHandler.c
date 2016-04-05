/*
 * A class to represent an image and calculate which
 * diods should light up given the current position.
 */

#include <stdint.h>
#include "ImageHandler.h"
#include "OutputInterpreter.h"

#define IMG_LEN (imgs[currImg][0])

static void updateImgChoice();

static int currImg = 1;
static int lastIndex = 0;

#define NUM_IMAGES 6

unsigned int *imgs[NUM_IMAGES] =
		{
			//"  TIFO!  "
			(unsigned int[])
			{44,0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
				0x0000, 0xc000, 0xc000, 0xffff, 0xc000, 0xc000, 0x0000,      	//T
				0x0000, 0xc003, 0xffff, 0xc003, 0x0000,                  		//I
				0x0000, 0xffff, 0xc0c0, 0xc0c0, 0xc0c0, 0xc0c0, 0xc000, 0x0000,	//F
				0x0000, 0x3ffc, 0x6006, 0xc003, 0xc003, 0x6006, 0x3ffc, 0x0000,	//O
				0x0000, 0xfff3, 0xfff3, 0x0000,                       			//!
				0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
			//Double Romb "<<>>"
			(unsigned int[])
			{32,0x00018000, 0x00024000, 0x00042000, 0x00081000, 0x00100800, 0x00200400, 0x00400200, 0x00800100, //<
				0x01018080, 0x02024040, 0x04042020, 0x08081010, 0x10100808, 0x20200404, 0x40400202, 0x80800101, //<<
				0x80800101, 0x40400202, 0x20200404, 0x10100808, 0x08081010, 0x04042020, 0x02024040, 0x01018080, //>>
				0x00800100, 0x00400200, 0x00200400, 0x00100800, 0x00081000, 0x00042000, 0x00024000, 0x00018000},//>
			//"  HEJA IT!  "
			(unsigned int[])
			{64,0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
				0x0000, 0xffff, 0x0180, 0x0180, 0xffff, 0x0000,					//H
				0x0000, 0xffff, 0xc0c3, 0xc0c3, 0xc0c3, 0xc0c3, 0xc003, 0x0000,	//E
				0x0000, 0x000c, 0x0006, 0x0003, 0x0003, 0xc006, 0xfffc, 0x0000,	//J
				0x0000, 0x01ff, 0x3fc0, 0xc0c0, 0xc0c0, 0x3fc0, 0x01ff, 0x0000, //A
				0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
				0x0000, 0xc003, 0xffff, 0xc003, 0x0000,                  		//I
				0x0000, 0xc000, 0xc000, 0xffff, 0xc000, 0xc000, 0x0000,      	//T
				0x0000, 0xfff3, 0xfff3, 0x0000,                       			//!
				0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
			//Smiley
			(unsigned int[])
			{18,0x0000, 0x07e000, 0x1ff800, 0x3fdc00, 0x79cc00, 0x70e600, 0xf0e700, 0xf9e300, 0xffe300,	//Leftface
				0xffe300, 0xf9e300, 0xf0e700, 0x70e600, 0x79cc00, 0x3fdc00, 0x1ff800, 0x07e000, 0x0000},//Rightface
			//Thumbs up
			(unsigned int[])
			{18,0x0000, 0x00fe, 0x00fe, 0x01ff, 0x03ff, 0x07ff, 0x0fff, 0x1fff, 0x7fff,
				0xffff, 0xffff, 0x73ff, 0x03ff, 0x03fe, 0x03fc, 0x01f0, 0x00c0, 0x0000},
			//Thumbs down
			(unsigned int[])
			{18,0x0000, 0x7f00, 0x7f00, 0xff80, 0xffc0, 0xffe0, 0xfff0, 0xfff8, 0xfffe,
				0xffff, 0xffff, 0xffce, 0xffc0, 0x7fc0, 0x3fc0, 0x0f80, 0x0300, 0x0000}
		};

void initImageHandler(){
	initOutputs();
}

/*
 * This function determines which part of the image or message to print out.
 * Also sends the determined column to the updateOutputs function.
 */
void updateImage(float pos) {
	//Check for updates on image choice
	updateImgChoice();

	// Compute which column to print
	int currIndex = pos*IMG_LEN;

	// If index is out of range (lower), set index to first possible position
	if (currIndex < 0) {
		currIndex = 0;
	// If index is out of range (higher), set index to last possible position
	} else if (currIndex >= IMG_LEN) {
		currIndex = IMG_LEN - 1;
	}
	// If current index differs from last index
	if (currIndex != lastIndex) {
		// Set current index to last index
		lastIndex = currIndex;
		// Send current column to output
		updateOutputs(imgs[currImg][currIndex+1]); //First value is the length of the array
	}
}

void accelDrawer(int16_t val) {
	int nbrOfLights = (((float)val)/32767)*16;
	int pixels = 0;
	int i = (nbrOfLights<0 ? -nbrOfLights : nbrOfLights);
	for(; i>0;i--) {
		if (nbrOfLights<0) {
			pixels <<= 1;
			pixels |= 0x10000;
		} else {
			pixels >>= 1;
			pixels |= 0x8000;
		}
	}
	updateOutputs(pixels);
}

void updateImgChoice() {
	//TODO Read from ports which pins are active.
	currImg = currImg;
}

void nextImage() {
	currImg = (currImg+1)%NUM_IMAGES;
}

void previousImage() {
	currImg = (currImg+NUM_IMAGES-1)%NUM_IMAGES;
}
