#include "utils.h"
#include "linkedList.h"
#include "ponto.h"
#include "globals.h"
#include "pixel.h"
#include "lago.h"
#include "estatistics.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <omp.h>
#include <math.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

/* ========================== VARIAVEIS GLOBAIS =========================== */
struct timeval iniciostep; 		  // contagem do tempo a cada timestep
struct timeval t0;				  // contagem da simulação
double R;
ALLEGRO_BITMAP *bmp = NULL;

#define FRACAO_FAIXA    0.70

/* ========================== PROTOTIPOS =========================== */ 
void simula();
int initialize();
void initializeGlobals();

void geraCirculo(PONTO* centro, double r);
int temProximaGota();
void varreFaixa(int linIn, int linF, int colIn, int colF);
void varreCirculo(float cx, float cy, float r);

/* =================================================================== */ 
 
int main(int argc, char **argv) {
	FILE* entrada;
	if (argc < 3) { 
		fprintf(stderr, "formato exigido: ./ep <arquivo> <raio>\n");
		return 0; 
	}
	entrada = fopen(argv[1], "r");
	
	if (!entrada) {
		fprintf(stderr, "ERRO ao abrir arquivo %s .\n", argv[1]); 
		exit(0);
	}

	fscanf(entrada,"(%d,%d)\n(%d,%d)\n%f\n%f\n%f\n%d\n%f\n%d\n", 
		&larg, &alt, &L, &H, &T, &v, &epsilon, &NIT, &prob, &semente);

	R = atof(argv[2]);
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
	draio = 0.0;
	dt = (float)(draio/v);
	timestep = (float)(T/NIT);
	
	printf("pixelHigh = %f\n", pixelHigh);
	printf("pixelWidth = %f\n", pixelWidth);
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
        fprintf(stderr, "Falha ao inicializar image_addon().\n");
        return 0;
    }

    bmp = al_create_bitmap(L, H);
    int h = al_get_bitmap_height(bmp);
    int w = al_get_bitmap_width(bmp);
    printf("height = %d\n", h);
    printf("width = %d\n\n", w);
    
    return 1;
}

void simula() {
	PONTO centro;
	centro.x = (double) (L/2) * pixelWidth;
	centro.y = (double) (H/2) * pixelHigh;

	geraCirculo(&centro, R);
	defineCorPixels();
	geraPPM("ondas.ppm");
}

void geraCirculo(PONTO* centro, double r) {
	al_set_target_bitmap(bmp);
    al_clear_to_color(al_map_rgb(255,255,255));
    al_draw_circle(centro->x, centro->y, r, al_map_rgb(250, 0, 0), 1.0);
    
    varreCirculo(centro->x, centro->y, r);
}

void varreCirculo(float cx, float cy, float r) {
    varreFaixa(floor(cy-FRACAO_FAIXA*r), ceil(cy+FRACAO_FAIXA*r), 
        floor(cx-r-2), ceil(cx-FRACAO_FAIXA*r));
    
    varreFaixa(floor(cy-r-2), ceil(cy-FRACAO_FAIXA*r), floor(cx-r), ceil(cx+r));
    varreFaixa(floor(cy+FRACAO_FAIXA*r), ceil(cy+r+2), floor(cx-r), ceil(cx+r));
    
    varreFaixa(floor(cy-FRACAO_FAIXA*r), ceil(cy+FRACAO_FAIXA*r), 
        floor(cx+FRACAO_FAIXA*r), ceil(cx+r+2));
}

void varreFaixa(int linIn, int linF, int colIn, int colF) {
    int y, x;
    unsigned char R;
    unsigned char G;
    unsigned char B;
    ALLEGRO_COLOR cor;

    for (y = linIn; y <= linF; y++) {
        for (x = colIn; x <= colF; x++) {
        	if (x >= 0 && y >= 0 && x < L && y < H) {
	            cor = al_get_pixel(bmp, x, y);
	            al_unmap_rgb(cor, &R, &G, &B);

	            if (R == 250) {
	                cor = al_map_rgb(0, 0, 0);
	                //al_put_pixel(x, y, cor);
	            	lago[y][x].pto.h = 50;
            	}
        	}
        }
    }
}

// probabilidade deve ser um flutuante entre 0 e 100, inclusive
int temProximaGota() {
	Randomize(semente);

	if (RandomReal(0, 1) < (0.01 * prob)) return 1;
	return 0;
}