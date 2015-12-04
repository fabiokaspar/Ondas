#ifndef PONTO_H
#define PONTO_H

#include "linkedList.h"

typedef struct ps {
	int lin;
	int col;
} POSITION; // linha e coluna do lago

typedef struct ponto {
	float x;
	float y; 
	float h;
	POSITION self; // ponto dele mesmo no lago
	POSITION** circle; // circulo da onda tendo o ponto como centro
	Link radiais;
} PONTO;  // no plano cartesiano


PONTO polarEmCartesiano(float, float, PONTO*);
PONTO sorteiaPonto(float xmax, float ymax);
PONTO setaPonto(float x, float y, float h, Link radiais);
int pontoEstaDentro(PONTO*);

#endif