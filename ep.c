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

#define MAXGOTA							100

/* ========================== VARIAVEIS GLOBAIS =========================== */
struct timeval iniciostep; 		  // contagem do tempo a cada timestep
struct timeval t0;				  // contagem da simulação

int nradiais;

POSITION chuva[MAXGOTA];  // vetor que mapeia as posições da matriz lago onde surgem gotas
int indexG = 0; 		  // inicialmente nenhuma gota caiu

/* ========================== PROTOTIPOS =========================== */ 
void parser(FILE*, char*);
float calculaAltura(float, float);
void simula();
void propagaOndaCircular(PONTO*);
void iniciaFocoDeOnda(PONTO* centro);
void initializeVariables();
void geraArquivoEstatistico();

void removeGota(int posRemove);
void insereGota(POSITION nova);
int existeGota();
int temProximaGota();
void novaGota(PONTO* gota);
/* =================================================================== */ 
 
int main(int argc, char **argv) {
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
	initializeVariables();

	omp_set_num_threads(nprocs);

	inicializaPontosLago();
	inicializaImagem();

	simula();
	
	fclose(entrada);
	liberaImagem();
	liberaLago();
	return 0;
}

void initializeVariables() {
	// 1/20 é o espaçamento entre as celulas da matriz
	ALT = 20 * alt;
	LARG = 20 * larg;
	draio = (float) 0.025; // metade de 1/20
	dtheta = (float)(M_PI * 0.01);
	nradiais = 2 * (int)(M_PI/dtheta);
	// tempo de deslocamento em draio da frente de onda
	dt = (float)(draio/v);
	timestep = (float)(T/NIT);
}

void geraArquivoEstatistico() {
	FILE* arq = fopen("estatistica.txt", "w");
	int i, j;
	float media, dp;

	fprintf(arq, "pos.x | pos.y | media | desvio_padrao\n\n");

	for (i = 0; i < ALT; i++) 
	{
		#pragma omp parallel for private(media, dp) 
			for (j = 0; j < LARG; j++) 
			{
				media = 0;
				dp = 0;
				if (lago[i][j].alturas->total) {
					media = average(lago[i][j].alturas);
					dp = standardDeviation(lago[i][j].alturas, media);
					
					if (dp != 0 || media != 0) {
						#pragma omp critical
						{	fprintf(arq, "%12.7f | %12.7f | %12.7f | %12.7f\n", 
								lago[i][j].x, lago[i][j].y, media, dp);	
						}
					}
				}		
			}
	}
	printf("estatistica.txt gerado com sucesso\n");
	fclose(arq);
}

// função do enunciado
float calculaAltura(float r, float t) {	
	double a = (double)r - (double)v * (double)t;
	double b = (-1) * pow(a, 2);
	b = exp(b);
	double c = exp((-0.1) * t);
	double h = a * b * c;

	return ((float)h);
}

void novaGota(PONTO* gota) {
	mapeiaPontoParaLago(gota);  // localiza o ponto dentro da matriz
	iniciaFocoDeOnda(gota);		
	atualizaPontoNoLago(gota); 
	insereGota(gota->self);  	// insere no vetor chuva
}

void simula() {
	int i;
	int vaiChover = 0;
	PONTO gota;
	
	gettimeofday(&t0, NULL); // inicio da simulação
	
	for (i = 0; i < NIT; i++) 
	{
		printf("# iteracao: %d\n", i);
		vaiChover = temProximaGota();
		
		if (vaiChover) {
			gota = sorteiaPonto(larg - 0.1, alt - 0.1);
			novaGota(&gota);
		}
		
		gettimeofday(&iniciostep, NULL);
		// timestep eh o tempo maximo da iteração
		while (tempoDesdeInicio(iniciostep) < timestep) {			
			if (existeGota()) {		
				// função executada em paralelo
				propagaOndaCircular(&gota);
				removeGota(0);
			}
			else break; // se não há o que fazer, iteração acabou
		}	
		printf("Tempo de duração [it. # %d]: %.3f seg\n\n", i, tempoDesdeInicio(iniciostep));	
	}

	printf("TEMPO TOTAL: %.3f seg\n", tempoDesdeInicio(t0));
	defineCorPixels();
	geraPPM("ondas.ppm");
	geraArquivoEstatistico();	
}

// Cada raio é calculado por uma thread 
// Cada thread calcula uma classe de equivalencia "inteira" da circunferencia
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

	nprocs = atoi(argv2);

	if (nprocs <= 0) { 
		nprocs = 1;
	}
}

// probabilidade deve ser um flutuante entre 0 e 100, inclusive
int temProximaGota() {
	Randomize(semente);

	if (RandomReal(0, 1) < (0.01 * prob)) return 1;
	return 0;
}

/* ========================== Funções da gota =========================== */ 
// inserir novo foco de onda
void insereGota(POSITION nova) {
	if (indexG < MAXGOTA) {
		chuva[indexG++] = nova;
	}
}

void removeGota(int posRemove) {
	int i;
	for (i = posRemove; i < indexG-1; i++) {
		chuva[i] = chuva[i+1];
	}
	indexG--;
}

int existeGota() {
	if (indexG)
		return 1;
	return 0;
}