#ifndef GLOBALS_H
#define GLOBALS_H

#include "node.h"

int semente, NIT;
int nprocs;
double epsilon, prob, v, T;
double hmax, pmax; 	// altura e profundidade max.


int L, H; 			// matriz de pixels
int larg, alt; 		
double pixelWidth, pixelHigh;

// DAQUI PARA FRENTE:
// RAIO e ANGULO sao relativos ao ponto de queda da gota

double timestep;

NODE **node;		// matriz node

#endif