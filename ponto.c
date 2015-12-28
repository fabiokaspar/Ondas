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

PONTO polarEmCartesiano(POLAR* polar) {
	PONTO p;

	double r = polar->raio;
	double cosTh = cos (polar->theta);
	double sinTh = sin (polar->theta);
		
	p.x = polar->origem.x + r * cosTh; 
	p.y = polar->origem.y + r * sinTh; 

	return p;
}

double calculaDistancia(PONTO* p1, PONTO* p2) {
	double d;

	d = sqrt(pow(p1->x - p2->x , 2) + pow(p1->y - p2->y, 2));
	return d;	
}

POLAR cartesianoEmPolar(PONTO* cartesiano, PONTO* origem) {
	POLAR p;
	double y = cartesiano->y;
	double Oy = origem->y;

	p.raio = calculaDistancia(cartesiano, origem);
	p.theta = asin((y-Oy)/p.raio);
	p.origem = (*origem);	

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