#include <math.h>
#include <stdio.h>
#include "pixel.h"
#include "utils.h"
#include "globals.h"

void defineCorPixels() {
	float delta, h;
	int i, j;

	pmax = (-1) * pmax;

	if (hmax >= pmax) 
		delta = hmax;
	else 
		delta = pmax;

	delta = (float)(delta/(float)255);

	if (delta == 0)
		delta = 1;

	for (i = 0; i < H; i++) {
		for (j = 0; j < L; j++) {
			h = imagem[i][j].pto->h;
			
			if (h == 0) {
				imagem[i][j].red = imagem[i][j].green = imagem[i][j].blue = 255;
			}
			
			else if (h > 0) {
				imagem[i][j].red = imagem[i][j].green = 0;
				imagem[i][j].blue =	((int) ceil((double)(h/delta)));
				//imagem[i][j].blue =	0;
			}

			else {
				h *= (-1);
				imagem[i][j].blue = imagem[i][j].green = 0;
				imagem[i][j].red = ((int) ceil((double)(h/delta)));
				//imagem[i][j].red = 0;
			}
		}
	}
}

void inicializaImagem() {
	imagem = (PIXEL**) mallocSafe(H * sizeof(PIXEL*));
	int i, j;

	for (i = 0; i < H; i++) {
		imagem[i] = mallocSafe(L * sizeof(PIXEL));

		for (j = 0; j < L; j++)
			imagem[i][j] = (PIXEL){255, 255, 255, &lago[i][j]};
	}
}

void liberaImagem() {
	int i;
	for (i = 0; i < H; i++)
		free(imagem[i]);	
}

void geraPPM(char* fname) {
	FILE* arq = fopen(fname, "w");
	int i, j;
	fprintf(arq, "P3\n");
	fprintf(arq, "%d %d\n", L, H);
	fprintf(arq, "255");

	for (i = 0; i < H; i++) {
		for (j = 0; j < L; j++) {
			fprintf(arq, "\n%d %d %d", imagem[i][j].red, imagem[i][j].green, imagem[i][j].blue);
		}
	}
	fclose(arq);
}
