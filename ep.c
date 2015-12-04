#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <sys/time.h>
#include "utils.h"
#include "linkedList.h"
#include "ponto.h"
#include "globals.h"
#include "pixel.h"
#include "lago.h"
#include <omp.h>

/* ========================== VARIAVEIS GLOBAIS =========================== */
struct timeval t0;
int nradiais;
float dt;
	
/* ========================== PROTOTIPOS =========================== */ 
void parser(FILE*, char*);
float calculaAltura(float, float);
void TesteGota();
int temProximaGota();
void propagaOndaCircular(PONTO*);
void propagaOndaPorNivel(float, float, PONTO*);
void inicializaFocoOnda(PONTO* centro);
/*=================================================================== */ 
 
int main(int argc, char **argv) {
	gettimeofday(&t0, NULL);

	FILE* entrada;
	if (argc < 3) { 
		fprintf(stderr, "formato exigido: ./ep <arquivo> <#processadores>\n");
		return 0; 
	}
	entrada = fopen(argv[1], "r");
	
	if (!entrada) {
		fprintf(stderr, "ERRO ao abrir arquivo %s .\n", argv[1]); 
		exit(0);
	}

	parser(entrada, argv[2]);
	
	omp_set_num_threads(nprocs);
	printf("Numero de threads: %d\n", nprocs);

	inicializaPontosLago();
	inicializaImagem();

	TesteGota();
	defineCorPixels();
	geraPPM("ondaTeste.ppm");

	fclose(entrada);
	liberaImagem();
	liberaLago();
	return 0;
}

float calculaAltura(float r, float t) {	
	double a = (double)r - (double)v * (double)t;
	double b = (-1) * pow(a, 2);
	b = exp(b);
	double c = exp((-0.1) * t);
	double h = a * b * c;

	return ((float)h);
}

void TesteGota() {
	PONTO gota;
	gota = sorteiaPonto(larg - 0.5, alt - 0.5);
	
	inicializaFocoOnda(&gota);
	mapeiaPontoParaLago(&gota);
	atualizaPontoNoLago(&gota);
	propagaOndaCircular(&gota);	
}

// NIVEL = TODAS AS DIRECOES COM MESMO RAIO
void propagaOndaPorNivel(float raio, float tempo, PONTO* centro) {
	float th;
	int lin, col;
	PONTO p;
	
	lin = (int)(raio/(9 * draio));

	for (th = 0; th < 2 * M_PI; th += dtheta) {
		if (!searchItem(centro->radiais, th)) { // direcao ja foi propagada ?
			p = polarEmCartesiano(raio, th, centro);
		
			if (pontoEstaDentro(&p)) {
				mapeiaPontoParaLago(&p);
				p.h = calculaAltura(raio, tempo);
				atualizaPontoNoLago(&p);
				
				col = (int)(th/dtheta);
				centro->circle[lin][col] = p.self;
			} else {
				// direcao ja foi propagada: criterio de parada
				insertItem(centro->radiais, th);
			}
		}
	}
}

void inicializaFocoOnda(PONTO* centro) {
	int i, j;
	int R = raioMax();

	centro->radiais = initialize();
	centro->h = calculaAltura(0, 0);
	centro->circle = (POSITION**) mallocSafe(R * sizeof(POSITION*));

	for (i = 0; i < R; i++) {
		centro->circle[i] = (POSITION*) mallocSafe(nradiais * sizeof(POSITION));

		for (j = 0; j < nradiais; j++) {
			centro->circle[i][j] = (POSITION) {0, 0};
		}
	}
}

void propagaOndaCircular(PONTO* centro) {
	float r = 0, t = 0;
	
	while (centro->radiais->total < nradiais) {
		// precisa dar tempo de chegar no proximo raio
		r += 9 * draio;
		t += 9 * dt;
		dorme(9 * dt);
		propagaOndaPorNivel(r, 0.1 * t, centro);
	}
	freeAll(centro->radiais);
}

void parser(FILE* entrada, char* argv2) {
	fscanf(entrada,"(%d,%d)\n(%d,%d)\n%d\n%f\n%f\n%d\n%f\n%d\n", 
		&larg, &alt, &L, &H, &T, &v, &epsilon, &NIT, &prob, &semente);

	printf("\nlarg = %d\n", larg);
	printf("alt = %d\n", alt);
	printf("L = %d\n", L);
	printf("H = %d\n", H);
	printf("T = %d\n", T);
	printf("v = %f\n", v);
	printf("epsilon = %f\n", epsilon);
	printf("NIT = %d\n", NIT);
	printf("prob = %f\n", prob);
	printf("semente = %d\n", semente);
	
	nprocs = atoi(argv2);

	if (nprocs <= 0) { 
		nprocs = 1;
	}

	ALT = 10 * alt;
	LARG = 10 * larg;
	draio = (float)0.050;
	dtheta = (float)(M_PI * 0.01);
	H = ALT;
	L = LARG;
	nradiais = 2 * (int)(M_PI/dtheta);
	dt = (float)(draio/v);
}

int temProximaGota() {
	Randomize(semente);

	if (RandomReal(0, 1) < 0.01 * prob) return 1;
	return 0;
}