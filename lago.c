#include <math.h>
#include <stdio.h>
#include "utils.h"
#include "lago.h"
#include "globals.h"

void atualizaPontoNoLago(PONTO* foco) {
	int col = (int)(foco->x/(2*draio));
	int lin = (int)(foco->y/(2*draio));
	float modH;

	lago[lin][col].h += foco->h;
	//modH = (float) fabs ((double)lago[lin][col].h);

	//if (modH < epsilon)
	//	lago[lin][col].h = 0;

	if (lago[lin][col].h > hmax)
		hmax = lago[lin][col].h;

	if (lago[lin][col].h < pmax)
		pmax = lago[lin][col].h;
}

void inicializaPontosLago() {
	int i, j, Cx, Cy;
	float x, y;
	lago = (PONTO**) mallocSafe(ALT * sizeof(PONTO*));

	Cy = 1;
	for (i = 0; i < ALT; i++) {
		lago[i] = mallocSafe(LARG * sizeof(PONTO));

		y = Cy * draio;
		Cx = 1;
		for (j = 0; j < LARG; j++) {			
			x = Cx * draio;
			Cx += 2;
			
			lago[i][j] = (PONTO){x, y, 0};
		}
		Cy += 2;
	}
}

void imprimeMatrizLago() {
	int i, j;

	for (i = 0; i < ALT; i++) {
		for (j = 0; j < LARG; j++) {
			printf("(%f, %f) ", lago[i][j].x, lago[i][j].y);
		}
		printf("\n");
	}
}

void liberaLago() {
	int i;
	for (i = 0; i < ALT; i++)
		free(lago[i]);
}
