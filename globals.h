#ifndef GLOBALS_H
#define GLOBALS_H

#include "pixel.h"

int semente, NIT;
int nprocs;
float epsilon, prob, v, T;
int larg, alt; 				// dimensoes reais do lago (base de cálculo das matrizes)

// DAQUI PARA FRENTE:
// RAIO e ANGULO sao relativos ao ponto de queda da gota

float draio;
float dtheta;
float hmax, pmax; 	// altura e profundidade max.
int LARG, ALT; 		// dimensões da matriz lago, que é EXATAMENTE 20 x o tamanho do lago
int L, H; 			// matriz de pixels é no MÁXIMO 20 x o tamanho do lago

float dt; 			// tempo de deslocamento em draio da frente de onda
float timestep;

PIXEL** imagem;		// matriz que mapeia exatamente do lago
PONTO** lago;		// matriz lago

#endif