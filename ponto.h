#ifndef PONTO_H
#define PONTO_H

#include "linkedList.h"
#include "pixel.h"

// NO PLANO CARTESIANO
typedef struct ponto {
	double x, y, h;
} PONTO;

PONTO sorteiaPonto(float xmax, float ymax);
int pontoEstaDentro(PONTO* x);

#endif