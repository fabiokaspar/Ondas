#ifndef PONTO_H
#define PONTO_H

#include "linkedList.h"

typedef struct ps {
	int lin;
	int col;
} POSITION; // linha e coluna do lago

// NO PLANO CARTESIANO
typedef struct ponto {
	float x;
	float y; 
	float h;
	POSITION self; // posição do ponto no lago: util para atualizar o h do ponto no lago
	Link alturas;  // para armazenar o historico das alturas
} PONTO;


PONTO polarEmCartesiano(float, float, PONTO*);
PONTO sorteiaPonto(float xmax, float ymax);
int pontoEstaDentro(PONTO*);

#endif