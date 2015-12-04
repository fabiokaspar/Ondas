#ifndef LAGO_H
#define LAGO_H

#include "ponto.h"

void inicializaPontosLago();
void atualizaPontoNoLago(PONTO*);
void imprimeMatrizLago();
void mapeiaPontoParaLago(PONTO* p);
int raioMax();
void liberaLago();

#endif