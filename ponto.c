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

// apenas sorteia um ponto cartesiano qualquer
PONTO sorteiaPonto(float xmax, float ymax) {
	PONTO novo;

	Randomize(semente);
	novo.x = RandomReal(xmax/2, xmax/2);
	novo.y = RandomReal(ymax/2, ymax/2);

	return novo;
}