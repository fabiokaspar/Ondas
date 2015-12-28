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
double R;

/* ========================== PROTOTIPOS =========================== */ 
void parser(FILE*, char*);
void simula();
void initialize();
void circleMidPoint(int xc, int yc, int r);

void geraCirculo(PONTO* centro, double r, double v);
void Circulo(int xc, int yc, int r);
void setPixel(int lin, int col);
int temProximaGota();

/* =================================================================== */ 
 
int main(int argc, char **argv) {
	FILE* entrada;
	if (argc < 3) { 
		fprintf(stderr, "formato exigido: ./ep <arquivo> raio\n");
		return 0; 
	}
	entrada = fopen(argv[1], "r");
	
	if (!entrada) {
		fprintf(stderr, "ERRO ao abrir arquivo %s .\n", argv[1]); 
		exit(0);
	}

	parser(entrada, argv[2]);
	initialize();

	//omp_set_num_threads(nprocs);

	inicializaPontosLago();

	simula();
	
	fclose(entrada);
	liberaLago();
	return 0;
}

void initialize() {
	pixelWidth = ((double)larg)/L;
	pixelHigh = ((double)alt)/H;
	draio = 0.0;
	dtheta = 0.01;
	dt = (float)(draio/v);
	timestep = (float)(T/NIT);
	printf("pixelHigh = %f\n", pixelHigh);
	printf("pixelWidth = %f\n", pixelWidth);
}

void simula() {
	PONTO c;
	c.x = (double) (L/2) * pixelWidth;
	c.y = (double) (H/2) * pixelHigh;

	geraCirculo(&c, R, 5);
	defineCorPixels();
	//Circulo((int)c.x, (int)c.y, (int)R);
	geraPPM("ondas.ppm");
}

//http://www.tutorialspoint.com/computer_graphics/circle_generation_algorithm.htm
void Circulo(int xc, int yc, int r) {
	int x = 0;
	int y = r;
	int p = 1 - r;
	
	setPixel(xc + x, yc + y);
	setPixel(xc - x, yc + y);
	setPixel(xc + x, yc - y);
	setPixel(xc - x, yc - y);
	setPixel(xc + y, yc + x);
	setPixel(xc - y, yc + x);
	setPixel(xc + y, yc - x);
	setPixel(xc - y, yc - x);

	while (x < y) {
		x++;
		if (p < 0) {
			p = p + 2*x + 1;
		}
		else {
			y--;
			p = p + 2*(x-y) + 1;
		}
		setPixel(xc + x, yc + y);
		setPixel(xc - x, yc + y);
		setPixel(xc + x, yc - y);
		setPixel(xc - x, yc - y);
		setPixel(xc + y, yc + x);
		setPixel(xc - y, yc + x);
		setPixel(xc + y, yc - x);
		setPixel(xc - y, yc - x);
	}
}

void setPixel(int lin, int col) {
	lago[lin][col].px = (PIXEL) {0, 0, 0};
	lago[lin][col+1].px = (PIXEL) {0, 0, 0};
	lago[lin][col-1].px = (PIXEL) {0, 0, 0};
	lago[lin+1][col].px = (PIXEL) {0, 0, 0};
	lago[lin+1][col+1].px = (PIXEL) {0, 0, 0};
	lago[lin+1][col-1].px = (PIXEL) {0, 0, 0};
	lago[lin-1][col].px = (PIXEL) {0, 0, 0};
	lago[lin-1][col+1].px = (PIXEL) {0, 0, 0};
	lago[lin-1][col-1].px = (PIXEL) {0, 0, 0};

	printf("lin:%d, col:%d\n", lin, col);
}


// le arquivo de entrada
void parser(FILE* entrada, char* argv2) {
	fscanf(entrada,"(%d,%d)\n(%d,%d)\n%f\n%f\n%f\n%d\n%f\n%d\n", 
		&larg, &alt, &L, &H, &T, &v, &epsilon, &NIT, &prob, &semente);

	//nprocs = atoi(argv2);
	R = atof(argv2);
}

// probabilidade deve ser um flutuante entre 0 e 100, inclusive
int temProximaGota() {
	Randomize(semente);

	if (RandomReal(0, 1) < (0.01 * prob)) return 1;
	return 0;
}

void geraCirculo(PONTO* centro, double r, double v) {
	NODE node;

	double d;
	double inix = centro->x - r - 30;
	double fimx = centro->x + r + 30;
	double iniy = centro->y - r - 30;
	double fimy = centro->y + r + 30;

	for (node.pto.y = iniy; node.pto.y <= fimy; node.pto.y += pixelHigh) {
		for (node.pto.x = inix; node.pto.x <= fimx; node.pto.x += pixelWidth) {
			d = pow(node.pto.x - centro->x, 2) + pow(node.pto.y - centro->y, 2); 
					
			if (d >= r * r && d < (r+3)*(r+3)) {
				node.pto.h = v;
				atualizaPontoNoLago(&node);
			}
		}
	}
}