#include "utils.h"
#include "linkedList.h"
#include "ponto.h"
#include "globals.h"
#include "pixel.h"
#include "lago.h"
#include "estatistics.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <sys/time.h>
#include <omp.h>


/* ========================== VARIAVEIS GLOBAIS =========================== */
struct timeval iniciostep; 		  // contagem do tempo a cada timestep
struct timeval t0;				  // contagem da simulação

/* ========================== PROTOTIPOS =========================== */ 
void parser(FILE*, char*);
float calculaAltura();
void simula();
void propagaOndaCircular(PONTO*);
void iniciaFocoDeOnda(PONTO* centro);
void initializeVariables();
void geraCirculo(PONTO* centro, float r);

int temProximaGota();

/* =================================================================== */ 
 
int main(int argc, char **argv) {
	FILE* entrada;
	if (argc < 2) { 
		fprintf(stderr, "formato exigido: ./ep <arquivo>\n");
		return 0; 
	}
	entrada = fopen(argv[1], "r");
	
	if (!entrada) {
		fprintf(stderr, "ERRO ao abrir arquivo %s .\n", argv[1]); 
		exit(0);
	}

	parser(entrada, NULL);
	initializeVariables();

	//omp_set_num_threads(nprocs);

	inicializaPontosLago();
	inicializaImagem();

	simula();
	
	fclose(entrada);
	liberaImagem();
	liberaLago();
	return 0;
}

void initializeVariables() {
	l = ((float)larg)/L;
	h = ((float)alt)/H;
	draio = (float) sqrt(l*l + h*h); 
	dtheta = (float)(M_PI * 0.001);
	dt = (float)(draio/v);
	timestep = (float)(T/NIT);
	printf("h = %f\n", h);
	printf("l = %f\n", l);
}

// função do enunciado
float calculaAltura() {	
	// double a = (double)r - (double)v * (double)t;
	// double b = (-1) * pow(a, 2);
	// b = exp(b);
	// double c = exp((-0.1) * t);
	// double h = a * b * c;

	// return ((float)h);
	return (float)50;
}

void simula() {
	PONTO c;
	//c = sorteiaPonto(larg, alt);
	c.x = (float)larg * 0.5;
	c.y = (float)alt * 0.5;

	geraCirculo(&c, 10);
	defineCorPixels();
	geraPPM("ondas.ppm");
}

void geraCirculo(PONTO* centro, float r) {
	float th;	
	PONTO p;

	for (th = 0.0; th < 2 * M_PI; th += dtheta) {
		p = polarEmCartesiano(r, th, centro);
		p.h = calculaAltura();
		mapeiaPontoParaLago(&p);
		atualizaPontoNoLago(&p);
	}
}

void propagaOndaCircular(PONTO* centro) {
	#pragma omp parallel
	{
		int id = omp_get_thread_num();
		float th;

		// olha todos os angulos th's pertinentes a esta thread
		for (th = id * dtheta; th < 2 * M_PI; th += nprocs * dtheta) 
		{
			float r = 0, t = 0;
			PONTO p;

			for (r = 9 * draio;  ;r += 9 * draio) {
				p = polarEmCartesiano(r, th, centro);
				if (!pontoEstaDentro(&p))
					break;
						
				// precisa dar tempo de chegar no proximo raio
				t += 9 * dt;
				dorme(9 * dt);
				
				mapeiaPontoParaLago(&p);
				
				// 0.1 é para fazer a onda aparecer, 
				//valores maiores de t "apagam" o seu contorno
				p.h = calculaAltura(r, 0.1 * t);  
				
				// matriz lago eh compartilhado
				#pragma omp critical 
				{
					atualizaPontoNoLago(&p);
				}
			}
		}
		printf("[Thread %d terminou]\n", id);	
	}
}

void iniciaFocoDeOnda(PONTO* centro) {
	centro->h = calculaAltura(0, 0);
}

// le arquivo de entrada
void parser(FILE* entrada, char* argv2) {
	fscanf(entrada,"(%d,%d)\n(%d,%d)\n%f\n%f\n%f\n%d\n%f\n%d\n", 
		&larg, &alt, &L, &H, &T, &v, &epsilon, &NIT, &prob, &semente);

	//nprocs = atoi(argv2);
}

// probabilidade deve ser um flutuante entre 0 e 100, inclusive
int temProximaGota() {
	Randomize(semente);

	if (RandomReal(0, 1) < (0.01 * prob)) return 1;
	return 0;
}

