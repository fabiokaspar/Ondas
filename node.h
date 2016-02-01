#ifndef NODE_H
#define NODE_H

#include "ponto.h"
#include "pixel.h"

typedef struct z2 {
	int lin;
	int col;
	double h;
} Z2;

typedef struct location {
	Z2 item;
	struct location* next;
} Location;

typedef struct node {
	PONTO pto;
	PIXEL px;
	Link hist;		// para armazenar o historico das alturas
	Location* loc;
} NODE;

int isEmptyLoc(Location* head);
void insertLoc(Location* head, Z2 item);
void removeLoc(Location* head);
Z2 getNextLoc(Location* head);
Location* initializeLoc();

void inicializaPontosLago();
void atualizaMatriz(int lin, int col, double h);
void imprimeMatrizLago();
void liberaLago();

#endif