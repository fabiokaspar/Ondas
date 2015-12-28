#ifndef LAGO_H
#define LAGO_H

#include "ponto.h"
#include "pixel.h"

typedef struct node {
	PONTO pto;
	PIXEL px;
	//Link hist;		// para armazenar o historico das alturas
} NODE;

void inicializaPontosLago();
void atualizaPontoNoLago(NODE*);
void imprimeMatrizLago();
void liberaLago();

#endif