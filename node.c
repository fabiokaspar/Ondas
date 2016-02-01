#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "node.h"
#include "linkedList.h"
#include "globals.h"

// incrementa com a altura ja existente (interferencia de ondas)
void atualizaMatriz(int lin, int col, double h) {	
	double fin;

	node[lin][col].pto.h += h;
	fin = node[lin][col].pto.h;

	if (fin * fin < epsilon * epsilon)
		node[lin][col].pto.h = 0.0;

	fin = node[lin][col].pto.h;
	
	if (fin > hmax)
		hmax = fin;
	
	if (fin < pmax)
		pmax = fin;
	            	
	// armazena no histórico de alturas do ponto
	insertItem(node[lin][col].hist, fin);
}

Location* initializeLoc() {
	Location* head = (Location*)mallocSafe(sizeof(Location));
	head->next = NULL;
	return head;
}

void insertLoc(Location* head, Z2 item) {
	Location* novo = (Location*) mallocSafe(sizeof(Location));
	novo->item = item;
	novo->next = head->next;
	head->next = novo;
}

int isEmptyLoc(Location* head) {
	return (head->next == NULL);
}

void removeLoc(Location* head) {
	Location *lixo;
	lixo = head->next;
	head->next = lixo->next;
	free(lixo);
}

Z2 getNextLoc(Location* head) {
	return head->next->item;	
}

void inicializaPontosLago() {
	int ix, iy;
	double cx, cy = 0.0;

	node = (NODE**) mallocSafe(H * sizeof(NODE*));

	for (iy = 0; iy < H; iy++) {
		node[iy] = mallocSafe(L * sizeof(NODE));

		cx = 0.0;
		for (ix = 0; ix < L; ix++) {			
			node[iy][ix] = (NODE) {(PONTO) {cx, cy, 0.0}, (PIXEL) {0, 0, 0}, initializeList(), initializeLoc()};
			cx += pixelWidth;
		}
		cy += pixelHigh;
	}
}

void imprimeMatrizLago() {
	int i, j;

	for (i = 0; i < H; i++) {
		for (j = 0; j < L; j++) {
			printf("(%f, %f) ", node[i][j].pto.x, node[i][j].pto.y);
		}
		printf("\n");
	}
}

void liberaLago() {
	int i;
	for (i = 0; i < H; i++)
		free(node[i]);
}
