#ifndef PIXELS_H
#define PIXELS_H

#include "ponto.h"

typedef struct pixel {
	int red;
	int green;
	int blue;
	PONTO* pto; // lago é mapeado pra imagem
} PIXEL;

void inicializaImagem();
void liberaImagem();
void defineCorPixels();
void geraPPM(char* fname);


#endif