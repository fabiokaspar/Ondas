#include "utils.h"
#include "linkedList.h"
#include "ponto.h"
#include "globals.h"
#include "pixel.h"
#include "node.h"
#include "estatistics.h"

#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <string.h>

#include <math.h>
#include <omp.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#define FRACAO_FAIXA    1.00

typedef struct gota {
	Z2* foco;
	double tempo;
	double raiomax;
	struct gota* next;
	struct gota* prev;
} GOTA;

/* ========================== VARIAVEIS GLOBAIS =========================== */
struct timeval iniciostep; 		  // contagem do tempo a cada timestep
GOTA* headGota;
ALLEGRO_BITMAP *bmp = NULL;
int nprocs;

/* ========================== PROTOTIPOS =========================== */ 
void initializeListGotas();
void insertGota(Z2* novo);
void removeGota(GOTA* anterior);
GOTA* getNextGota();

void simulacao();
int initialize();
void initializeGlobals();
void propagaOnda(GOTA* gota);
double altura(double r, double t);

void varreFaixa(int linIn, int linF, int colIn, int colF, double h, NODE* centro);
void anulaOndaAnterior(GOTA* gota);
void geraOnda(GOTA* gota, double r, double t);
void geraCirculo(NODE* centro, double r, double h);

int temProximaGota();
Z2 sorteiaProximaGota();
/* =================================================================== */ 
 
int main(int argc, char **argv) {
	FILE* entrada;
	if (argc < 3) { 
		fprintf(stderr, "formato exigido: ./ep <arquivo_entrada> <num_processadores>\n");
		return 0; 
	}
	
	entrada = fopen(argv[1], "r");
	nprocs = atoi(argv[2]);
	
	if (!entrada) {
		fprintf(stderr, "ERRO ao abrir arquivo %s .\n", argv[1]); 
		return -1;
	}

	fscanf(entrada,"(%d,%d)\n(%d,%d)\n%lf\n%lf\n%lf\n%d\n%lf\n%d\n", 
		&larg, &alt, &L, &H, &T, &v, &epsilon, &NIT, &prob, &semente);

	if (!initialize()) {
		return -1;
	}

	initializeGlobals();
	omp_set_num_threads(nprocs);

	inicializaPontosLago();

	simulacao();
	
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

void simulacao() {
	int step;
	static int cont = 1;
	char fname[50];
	double clk;
	GOTA* gota;
	Z2 foco;

	initializeListGotas();

	// sorteio da primeira gota
	if (temProximaGota()) {
		foco = sorteiaProximaGota();
		insertGota(&foco);
	}

	for (step = 1; step <= NIT; step++) {
		gettimeofday(&iniciostep, NULL);

		// propagação de ondas
		printf("[step: %d]\n", step);
		while ((clk = tempoDesdeInicio(iniciostep)) < timestep) {
			if (headGota->next != NULL) {
				// simulação
				gota = getNextGota();
			
				propagaOnda(gota);
			}
			else {
				printf("Lago em repouso...\n"); // lago em repouso
				int tm = (timestep-clk) * 1000000;
				usleep(tm);
			}
		}
		
		defineCorPixels();
		sprintf(fname, "out%d.ppm", cont++);
		geraPPM(fname);
				
		printf("timestep: %f seg\n\n", tempoDesdeInicio(iniciostep));

		// sorteio da proxima gota (se houver step)
		if (step < NIT-1 && temProximaGota()) {
			foco = sorteiaProximaGota();
			insertGota(&foco);
		}
	}
}

void propagaOnda(GOTA* gota) {
	double raio;
	struct timeval in;

	gettimeofday(&in, NULL);
	
	raio = v * gota->tempo;
	
	anulaOndaAnterior(gota);
	
	if (raio <= gota->raiomax) {
		geraOnda(gota, raio, gota->tempo);
		gota->tempo += tempoDesdeInicio(in);
	}
	else {
		removeGota(gota);
	}
}

void geraCirculo(NODE* centro, double r, double h) {
    double cx, cy;
	al_set_target_bitmap(bmp);
    al_clear_to_color(al_map_rgb(255, 255, 255));
    al_draw_circle(centro->pto.x, centro->pto.y, r, al_map_rgb(250, 0, 0), 2.0);
	    
    cx = centro->pto.x;
    cy = centro->pto.y;
    varreFaixa(floor(cy-FRACAO_FAIXA*r), ceil(cy+FRACAO_FAIXA*r), 
        floor(cx - FRACAO_FAIXA * r), ceil(cx + FRACAO_FAIXA * r), h, centro);
}

void varreFaixa(int linIn, int linF, int colIn, int colF, double h, NODE* centro) {
	int y, x;
	unsigned char R;
	unsigned char G;
	unsigned char B;
	ALLEGRO_COLOR cor;
	Z2 item;

    #pragma omp parallel for private(x, R, G, B, cor, item) schedule(dynamic)
		for (y = linIn; y <= linF; y++) {
			for (x = colIn; x <= colF; x++) {
				if (x >= 0 && y >= 0 && x < L && y < H) {
					#pragma omp critical
					{
						cor = al_get_pixel(bmp, x, y);
					}
					
					al_unmap_rgb(cor, &R, &G, &B);

					if (R == 250) {
						item.lin = y;
						item.col = x;
						item.h = h;
						atualizaMatriz(y, x, h);
						
						#pragma omp critical
						{	
							insertLoc(centro->loc, item);
						}	
					}
				}
			}
		}
}

void geraOnda(GOTA* gota, double r, double t) {
	double h;
	NODE centro = node[gota->foco->lin][gota->foco->col]; 

	if (r >= 1.5) {
		h = altura(r-1.5, t);
		geraCirculo(&centro, r-1.5, h);
		
		h = altura(r+1.5, t);
		geraCirculo(&centro, r+1.5, h);
	}
}

void initializeListGotas() {
	headGota = (GOTA*) mallocSafe(sizeof(GOTA));
	headGota->next = NULL;
	headGota->prev = NULL;
}

void insertGota(Z2* novo) {
	double rx, ry, cx, cy;
	
	GOTA* novaGota = (GOTA*) mallocSafe(sizeof(GOTA));
	
	cx = node[novo->lin][novo->col].pto.x;
	cy = node[novo->lin][novo->col].pto.y;

	rx = fmax(cx, larg-cx);
	ry = fmax(cy, alt-cy);

	novaGota->raiomax = sqrt(rx * rx + ry * ry); 
	novaGota->tempo = 0;
	novaGota->foco = novo;

	novaGota->next = headGota->next;
	novaGota->prev = headGota;

	headGota->next = novaGota;
}

void removeGota(GOTA* removida) {
	GOTA* anterior = removida->prev;
	GOTA* posterior = removida->next;
	anterior->next = posterior;

	if (posterior != NULL)
		posterior->prev = anterior;
	
	free(removida);
}

GOTA* getNextGota() {
	static GOTA* g = 0;
	if (g == 0) {
		g = headGota;
	}

	g = g->next;
	if (g == NULL) {
		g = headGota->next;
	}
	
	return g;
}

void anulaOndaAnterior(GOTA* gota) {
	Z2 item;
	NODE centro = node[gota->foco->lin][gota->foco->col]; 

	while (!isEmptyLoc(centro.loc)) {
		item = getNextLoc(centro.loc);
		
		if (item.h >= 0) {
			node[item.lin][item.col].pto.h -= item.h;
		}
		else if (item.h < 0) {
			node[item.lin][item.col].pto.h += fabs(item.h);
		}

		removeLoc(centro.loc);
	}
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

    bmp = al_create_bitmap(L, H);
    int h = al_get_bitmap_height(bmp);
    int w = al_get_bitmap_width(bmp);
    printf("height = %d\n", h);
    printf("width = %d\n\n", w);
    
    return 1;
}

double altura(double r, double t) {
	double h;
	double termo = r - v * t;
	double expoente = (-1) * (termo * termo + t * 0.1);

	h = termo * exp(expoente);
	return h;
}

Z2 sorteiaProximaGota() {
	Z2 item;
	Randomize(semente);

	int lin = RandomReal(0, alt)/pixelHigh;
	int col = RandomReal(0, larg)/pixelWidth;
	item.lin = lin;
	item.col = col;
	return item;
}

// probabilidade deve ser um flutuante entre 0 e 100, inclusive
int temProximaGota() {
	Randomize(semente);

	if (RandomReal(0, 1) <= (0.01 * prob)) return 1;
	return 0;
}