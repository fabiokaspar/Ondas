#ifndef GLOBALS_H
#define GLOBALS_H

#include "pixel.h"

int semente, NIT, T;
int nprocs;
float epsilon, prob, v;
int larg, alt;
// DAQUI PARA FRENTE:
// RAIO e ANGULO sao relativos ao ponto de queda da gota

float draio;
float dtheta;
float hmax, pmax; // altura e profundidade max.
int LARG, ALT;
int L, H; /* matriz de pixels */

PIXEL** imagem;
PONTO** lago;

#endif