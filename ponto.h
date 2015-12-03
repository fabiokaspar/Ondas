#ifndef PONTO_H
#define PONTO_H

#include "linkedList.h"

typedef struct ponto {
	float x;
	float y; 
	float h;
	Link radiais;
} PONTO;  // no plano cartesiano

PONTO polarEmCartesiano(float, float, PONTO*);
PONTO sorteiaPonto(float xmax, float ymax);
PONTO setaPonto(float x, float y, float h, Link radiais);
int pontoEstaDentro(PONTO*);

#endif