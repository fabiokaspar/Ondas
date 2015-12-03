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

PONTO sorteiaPonto(float xmax, float ymax) {
	PONTO novo;

	Randomize(semente);
	novo.x = RandomReal(0.5, xmax);
	novo.y = RandomReal(0.5, ymax);

	return novo;
}

PONTO setaPonto(float x, float y, float h, Link radiais) {
	PONTO novo;
	novo.x = x;
	novo.y = y;
	novo.h = h;
	novo.radiais = radiais;
	return novo;
}