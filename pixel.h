#ifndef PIXELS_H
#define PIXELS_H


typedef struct pixel {
	unsigned char R, G, B;	
} PIXEL;

void defineCorPixels();
void geraPPM(char* fname);

#endif