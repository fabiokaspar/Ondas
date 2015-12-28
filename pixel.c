#include <math.h>
#include <stdio.h>
#include "pixel.h"
#include "utils.h"
#include "globals.h"

// tira uma foto da matriz lago
void defineCorPixels() {
	// float delta;
	double h;
	int i, j;

	// pmax = (-1) * pmax;

	// if (hmax >= pmax) 
	// 	delta = hmax;
	// else 
	// 	delta = pmax;

	// delta = (float)(delta/(float)255);

	// if (delta == 0)
	// 	delta = 1;

	for (i = 0; i < H; i++) {
		for (j = 0; j < L; j++) {
			h = lago[i][j].pto.h;
			
			if (h == 0) {
				lago[i][j].px.R = lago[i][j].px.G = lago[i][j].px.B = 255;
			}
			
			else if (h > 0) {
				lago[i][j].px.R = lago[i][j].px.G = 0;
				lago[i][j].px.B = 0;
			}

			// else {
			// 	h *= (-1);
			// 	imagem[i][j].blue = imagem[i][j].green = 0;
			// 	imagem[i][j].red = ((int) ceil((double)(h/delta)));
			// }
		}
	}
}

// imprime a foto do lago no arquivo
void geraPPM(char* fname) {
	FILE* arq = fopen(fname, "w");
	int i, j;
	fprintf(arq, "P3\n");
	fprintf(arq, "%d %d\n", L, H);
	fprintf(arq, "255");

	for (i = 0; i < H; i++) {
		for (j = 0; j < L; j++) {
			fprintf(arq, "\n%d %d %d", lago[i][j].px.R, lago[i][j].px.G, lago[i][j].px.B);
		}
	}
	fclose(arq);
}
