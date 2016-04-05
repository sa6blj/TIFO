#ifndef __IMAGEHANDLER_H__
#define __IMAGEHANDLER_H__

/*
 * Updates the image based on the current position given as a float between 0 and 1.
 */
void updateImage(float pos);

/*
 * Initiates the imagehandler to set up all reasourses it needs.
 */
void initImageHandler(void);

/*
 * (Debug method) Draws the current acceleration affected by the stick.
 */
void accelDrawer(int16_t val);

/*
 * Goes to the next image predefined image.
 */
void nextImage(void);

/*
 * Goes to the previous image predefined image.
 */
void previousImage(void);

#endif
