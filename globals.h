#ifndef GLOBALS_H
#define GLOBALS_H

#include "lago.h"

int semente, NIT;
int nprocs;
float epsilon, prob, v, T;
float hmax, pmax; 	// altura e profundidade max.


int L, H; 			// matriz de pixels
int larg, alt; 		
double pixelWidth, pixelHigh;

// DAQUI PARA FRENTE:
// RAIO e ANGULO sao relativos ao ponto de queda da gota

float dt; 			// tempo de deslocamento em draio da frente de onda
float draio;
float dtheta;
float timestep;

NODE **lago;		// matriz lago

#endif