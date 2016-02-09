/*
 * main.c
 */
#include <stdio.h>
#include <time.h>

char text[32] = {0x00, 0x80, 0x80, 0xff, 0x80, 0x80, 0x00, 		   //T
				0x00, 0x81, 0xff, 0x81, 0x00, 					   //I
				0x00, 0xff, 0x88, 0x88, 0x88, 0x88, 0x80, 0x00,	   //F
				0x00, 0x7e, 0xc3, 0x81, 0x81, 0xc3, 0x7e, 0x00,    //O
				0x00, 0xfb, 0xfb, 0x00} ;						   //!



void updatePosition();
void updateImage(float position);
void updateOutputs(int i);


//CONTROLLER
//freuency
int main(void) {
	time_t currTime = 0;
	while (true){
		updatePosition()
		//sleep(1/frequecy)
	}
	return 0;
}

//INTERPRETER
void updatePosition(){

}

//IMAGEHANDLER
void updateImage(float position){
	
}

//OUTPUTINTERPRETER
void updateOutputs(int i){

}
