#include "utils.h"
#include "linkedList.h"
#include "ponto.h"
#include "globals.h"
#include "pixel.h"
#include "node.h"
#include "estatistics.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <omp.h>
#include <math.h>
#include <string.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

/* ========================== VARIAVEIS GLOBAIS =========================== */
struct timeval iniciostep; 		  // contagem do tempo a cada timestep
struct timeval t0;				  // contagem da simulação
ALLEGRO_BITMAP *bmp = NULL;

#define FRACAO_FAIXA    0.70

/* ========================== PROTOTIPOS =========================== */ 
void simula();
int initialize();
void initializeGlobals();
void propagaOnda(NODE* centro);
double altura(double r, double t);

void varreFaixa(int linIn, int linF, int colIn, int colF, double h, NODE* centro);
void anulaOndaAnterior(NODE* centro);
double geraOnda(NODE* centro, double r, double t);
void geraCirculo(NODE* centro, double r, double h);
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

	fscanf(entrada,"(%d,%d)\n(%d,%d)\n%lf\n%lf\n%lf\n%d\n%lf\n%d\n", 
		&larg, &alt, &L, &H, &T, &v, &epsilon, &NIT, &prob, &semente);

	//nprocs = atoi(argv[2]);

	if (!initialize()) {
		return -1;
	}

	initializeGlobals();
	//omp_set_num_threads(nprocs);

	inicializaPontosLago();

	simula();
	
	fclose(entrada);
	liberaLago();
	return 0;
}

void initializeGlobals() {     
	pixelWidth = ((double)larg)/L;     
	pixelHigh = ((double)alt)/H;     
	timestep = (double)(T/NIT);     
	hmax = 0;     
	pmax = 0;
	printf("pixelHigh = %f\n", pixelHigh);     
	printf("pixelWidth = %f\n", pixelWidth); 
}

void simula() {
	//node[H/2][L/2].pto.x = (double) (L/2) * pixelWidth; 
	//node[H/2][L/2].pto.y = (double) (H/2) * pixelHigh;

	propagaOnda(&node[H-200][L/4]);
}

void geraCirculo(NODE* centro, double r, double h) {
    double cx, cy;
	al_set_target_bitmap(bmp);
    al_clear_to_color(al_map_rgb(255,255,255));
    al_draw_circle(centro->pto.x, centro->pto.y, r, al_map_rgb(250, 0, 0), 2.0);
	    
    cx = centro->pto.x;
    cy = centro->pto.y;
    
    varreFaixa(floor(cy-FRACAO_FAIXA*r), ceil(cy+FRACAO_FAIXA*r), 
        floor(cx-r-2), ceil(cx-FRACAO_FAIXA*r), h, centro);
    
    varreFaixa(floor(cy-r-2), ceil(cy-FRACAO_FAIXA*r), floor(cx-r), ceil(cx+r), h, centro);
    varreFaixa(floor(cy+FRACAO_FAIXA*r), ceil(cy+r+2), floor(cx-r), ceil(cx+r), h, centro);
    
    varreFaixa(floor(cy-FRACAO_FAIXA*r), ceil(cy+FRACAO_FAIXA*r), 
        floor(cx+FRACAO_FAIXA*r), ceil(cx+r+2), h, centro);
}

void varreFaixa(int linIn, int linF, int colIn, int colF, double h, NODE* centro) {
    int y, x;
    unsigned char R;
    unsigned char G;
    unsigned char B;
    ALLEGRO_COLOR cor;
    Z2 item;

    for (y = linIn; y <= linF; y++) {
        for (x = colIn; x <= colF; x++) {
        	if (x >= 0 && y >= 0 && x < L && y < H) {
	            cor = al_get_pixel(bmp, x, y);
	            al_unmap_rgb(cor, &R, &G, &B);

	            if (R == 250) {
	         		atualizaMatriz(y, x, h);
	         		item.lin = y;
	         		item.col = x;
	         		insertLoc(centro->loc, item); 	
            	}
        	}
		}
	}
}

void anulaOndaAnterior(NODE* centro) {
	Z2 item;
	while(!isEmptyLoc(centro->loc)) {
		item = getNextLoc(centro->loc);
		node[item.lin][item.col].pto.h = 0;
		removeLoc(centro->loc);
	}
}

double geraOnda(NODE* centro, double r, double t) {
	double h;
	h = altura(r-1.5, t);
	geraCirculo(centro, r-1.5, h);
	h = altura(r+1.5, t);
	geraCirculo(centro, r+1.5, h);
	return h;
}

void propagaOnda(NODE* centro) {
	double raio = 0, t = 0;
	double rx = fmax(centro->pto.x, larg - centro->pto.x);
	double ry = fmax(centro->pto.y, alt - centro->pto.y);
	double raioMax = sqrt(rx * rx + ry * ry);
	char fname[50];
	int cont = 1;
	struct timeval in;
	
	gettimeofday(&in, NULL);
	
	while (raio < raioMax) {
		t += tempoDesdeInicio(in);
		gettimeofday(&in, NULL);

		raio = v * t;
		geraOnda(centro, raio, t);
		defineCorPixels();
		anulaOndaAnterior(centro);
		sprintf(fname, "out%d.ppm", cont);
		geraPPM(fname);
		cont++;
	}
}

// probabilidade deve ser um flutuante entre 0 e 100, inclusive
int temProximaGota() {
	Randomize(semente);

	if (RandomReal(0, 1) < (0.01 * prob)) return 1;
	return 0;
}

double altura(double r, double t) {
	double h;
	double termo = r - v * t;
	double expoente = (-1) * (termo * termo + t * 0.1);

	h = termo * exp(expoente);
	return h;
}

int initialize() {
	if (!al_init())
    {
        fprintf(stderr, "Falha ao inicializar a biblioteca Allegro.\n");
        return 0;
    }
 
    if (!al_init_primitives_addon())
    {
        fprintf(stderr, "Falha ao inicializar add-on de primitivas.\n");
        return 0;
    }

    if (!al_init_image_addon())
    {
        fprintf(stderr, "Falha ao inicializar image_addon.\n");
        return 0;
    }

    bmp = al_create_bitmap(L, H);
    int h = al_get_bitmap_height(bmp);
    int w = al_get_bitmap_width(bmp);
    printf("height = %d\n", h);
    printf("width = %d\n\n", w);
    
    return 1;
}