#include <math.h>
#include <stdio.h>
#include "utils.h"
#include "lago.h"
#include "linkedList.h"
#include "globals.h"

// o ponto no lago recebe o sinal da onda, atualizando sua altura
void atualizaPontoNoLago(NODE* node) {
	int lin = node->pto.y/pixelHigh;
	int col = node->pto.x/pixelWidth;

	// incrementa com a altura ja existente (interferencia de ondas)
	lago[lin][col].pto.h += node->pto.h; 
	// float modH = (float) fabs ((double)lago[lin][col].h);

	// if (modH < epsilon)
	// 	lago[lin][col].h = 0;

	// // armazena no histórico de alturas do ponto
	// insertItem(lago[lin][col].alturas, lago[lin][col].h);

	// if (lago[lin][col].h > hmax)
	// 	hmax = lago[lin][col].h;

	// if (lago[lin][col].h < pmax)
	// 	pmax = lago[lin][col].h;
}

void inicializaPontosLago() {
	int ix, iy;
	double cx, cy = 0.0;

	lago = (NODE**) mallocSafe(H * sizeof(NODE*));

	for (iy = 0; iy < H; iy++) {
		lago[iy] = mallocSafe(L * sizeof(NODE));

		cx = 0.0;
		for (ix = 0; ix < L; ix++) {			
			lago[iy][ix] = (NODE) {(PONTO) {cx, cy, 0.0}, (PIXEL) {255, 255, 255}};
			cx += pixelWidth;
		}
		cy += pixelHigh;
	}
}

void imprimeMatrizLago() {
	int i, j;

	for (i = 0; i < H; i++) {
		for (j = 0; j < L; j++) {
			printf("(%f, %f) ", lago[i][j].pto.x, lago[i][j].pto.y);
		}
		printf("\n");
	}
}

void liberaLago() {
	int i;
	for (i = 0; i < H; i++)
		free(lago[i]);
}
