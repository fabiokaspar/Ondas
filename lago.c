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
	float modH = (float) fabs ((double)lago[lin][col].h);

	if (modH < epsilon)
		lago[lin][col].h = 0;

	// armazena no histórico de alturas do ponto
	insertItem(lago[lin][col].alturas, lago[lin][col].h);

	if (lago[lin][col].h > hmax)
		hmax = lago[lin][col].h;

	if (lago[lin][col].h < pmax)
		pmax = lago[lin][col].h;
}

// atribui no proprio ponto sua posicao na matriz lago
void mapeiaPontoParaLago(PONTO* p) {
	int col = (int)(p->x/(2*draio));
	int lin = (int)(p->y/(2*draio));

	p->self.lin = lin;
	p->self.col = col;
}

void inicializaPontosLago() {
	int i, j, Cx, Cy;
	float x, y;
	lago = (PONTO**) mallocSafe(ALT * sizeof(PONTO*));

	// Cada celula do lago tem como o centro seu ponto de referência.
	// Para qualquer ponto cartesiano cujas coordenadas pertençam 
	// ao intervalo numérico da celula, tomamos o centro da célula como 
	// representação desse mesmo ponto.

	Cy = 1;
	for (i = 0; i < ALT; i++) {
		lago[i] = mallocSafe(LARG * sizeof(PONTO));

		y = Cy * draio;
		Cx = 1;
		for (j = 0; j < LARG; j++) {			
			x = Cx * draio;
			Cx += 2;

			// (x, y, h, self, alturas) é o centro da célula
			lago[i][j] = (PONTO){x, y, 0, (POSITION) {i, j}, initialize()};
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
