#include <math.h>
#include "utils.h"
#include "ponto.h"
#include "globals.h"

int pontoEstaDentro(PONTO* p) {
	if (p->x <= 0 || p->x >= larg)
		return 0;
	if (p->y <= 0 || p->y >= alt)
		return 0;
	return 1;
}

// THETA EM RADIANOS
PONTO polarEmCartesiano(float r, float theta, PONTO* origem) {
	PONTO p;

	float cosTh = (float) cos ((double)theta);
	float sinTh = (float) sin ((double)theta);
		
	p.x = origem->x + r * cosTh; 
	p.y = origem->y + r * sinTh; 

	return p;
}

// apenas sorteia um ponto cartesiano qualquer
PONTO sorteiaPonto(float xmax, float ymax) {
	PONTO novo;

	Randomize(semente);
	novo.x = RandomReal(xmax/2, xmax/2);
	novo.y = RandomReal(ymax/2, ymax/2);

	return novo;
}