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
void iniciaFocoOnda(PONTO* centro);
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
	gota = sorteiaPonto(larg-0.5, alt-0.5);
	propagaOndaCircular(&gota);	
}

// NIVEL = TODAS AS DIRECOES COM MESMO RAIO
void propagaOndaPorNivel(float raio, float tempo, PONTO* centro) {
	float th;
	PONTO p;

	for (th = 0; th < 2 * M_PI; th += dtheta) {
		if (!searchItem(centro->radiais, th)) { // direcao ja foi propagada ?
			p = polarEmCartesiano(raio, th, centro);
		
			if (pontoEstaDentro(&p)) {
				p.h = calculaAltura(raio, tempo);
				atualizaPontoNoLago(&p);
			} else {
				// direcao agora eh propagada
				insertItem(centro->radiais, th);
			}	
		}
	}
}

void iniciaFocoOnda(PONTO* centro) {
	centro->radiais = initialize();
	centro->h = calculaAltura(0, 0); 
	
	//centro foi sorteado, não está na matriz lago
	atualizaPontoNoLago(centro);
}

void propagaOndaCircular(PONTO* centro) {
	float r, t;

	iniciaFocoOnda(centro);

	r = 9 * draio;
	t = 9 * dt;
	while (centro->radiais->total < nradiais) {
		// precisa dar tempo de chegar no proximo raio
		//dorme(dt);
		propagaOndaPorNivel(r, 0.1*t, centro);
		r += 7 * draio;
		t += 7 * dt;
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