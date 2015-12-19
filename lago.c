#include <math.h>
#include <stdio.h>
#include "utils.h"
#include "lago.h"
#include "linkedList.h"
#include "globals.h"

// o ponto no lago recebe o sinal da onda, atualizando sua altura
void atualizaPontoNoLago(PONTO* foco) {
	int lin = foco->self.lin;
	int col = foco->self.col;

	// incrementa com a altura ja existente (interferencia de ondas)
	lago[lin][col].h += foco->h; 
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

// atribui no proprio ponto sua posicao na matriz lago
void mapeiaPontoParaLago(PONTO* p) {
	int lin = (p->y/h);
	int col = (p->x/l);

	// printf("lin = %d\n", lin);
	// printf("col = %d\n\n", col);
	p->self.lin = lin;
	p->self.col = col;
}

void inicializaPontosLago() {
	int i, j, Cx, Cy;
	float x, y;
	lago = (PONTO**) mallocSafe(H * sizeof(PONTO*));

	Cy = 1;
	for (i = 0; i < H; i++) {
		lago[i] = mallocSafe(L * sizeof(PONTO));

		y = Cy * draio;
		Cx = 1;
		for (j = 0; j < L; j++) {			
			x = Cx * draio;
			Cx += 2;

			// (x, y, h, self, alturas) é o centro da célula
			lago[i][j] = (PONTO){x, y, 0, (POSITION) {i, j}, NULL};
		}
		Cy += 2;
	}
}

void imprimeMatrizLago() {
	int i, j;

	for (i = 0; i < H; i++) {
		for (j = 0; j < L; j++) {
			printf("(%f, %f) ", lago[i][j].x, lago[i][j].y);
		}
		printf("\n");
	}
}

void liberaLago() {
	int i;
	for (i = 0; i < H; i++)
		free(lago[i]);
}
