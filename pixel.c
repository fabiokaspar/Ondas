#include <stdio.h>
#include <math.h>
#include <omp.h>
#include "pixel.h"
#include "utils.h"
#include "globals.h"

// tira uma foto da matriz node
void defineCorPixels() {
	double delta;
	double h;
	int i, j;

	delta = (double) fmax((-1) * pmax, hmax);

	delta = delta/255;

	if (delta == 0)
		delta = 1;

	#pragma omp parallel for private(j, h) schedule(dynamic)
		for (i = 0; i < H; i++) {
			for (j = 0; j < L; j++) {
				h = node[i][j].pto.h;
				
				if (h == 0) {
					node[i][j].px.R = node[i][j].px.G = node[i][j].px.B = 0;
				}
				
				else if (h > 0) {
					node[i][j].px.R = node[i][j].px.G = 0;
					node[i][j].px.B = (unsigned char)ceil(h/delta);
				}

				else {
				 	h *= (-1);
				 	node[i][j].px.B = node[i][j].px.G = 0;
				 	node[i][j].px.R = (unsigned char)ceil(h/delta);
				}
			}
		}
}

// imprime a foto do node no arquivo
void geraPPM(char* fname) {
	FILE* arq = fopen(fname, "w");
	int i, j;
	fprintf(arq, "P3\n");
	fprintf(arq, "%d %d\n", L, H);
	fprintf(arq, "255");

	for (i = 0; i < H; i++) {
		for (j = 0; j < L; j++) {
			fprintf(arq, "\n%d %d %d", node[i][j].px.R, node[i][j].px.G, node[i][j].px.B);
		}
	}
	fclose(arq);
}
