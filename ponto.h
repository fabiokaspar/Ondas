#ifndef PONTO_H
#define PONTO_H

#include "linkedList.h"
#include "pixel.h"

// NO PLANO CARTESIANO
typedef struct ponto {
	double x, y, h;
} PONTO;

typedef struct polar {
	double raio, theta; 
	PONTO origem;
} POLAR;

PONTO polarEmCartesiano(POLAR* polar);
POLAR cartesianoEmPolar(PONTO* cartesiano, PONTO* origem);
PONTO sorteiaPonto(float xmax, float ymax);
int pontoEstaDentro(PONTO* x);
double calculaDistancia(PONTO* p1, PONTO* p2);

#endif