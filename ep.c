#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include "utils.h"

typedef struct ponto {
	float x;
	float y; 
	float h;
} PONTO;

typedef struct pixel {
	int red;
	int green;
	int blue;
	PONTO* pto; // lago é mapeado pra imagem
} PIXEL;

/*############ VARIAVEIS GLOBAIS ############# */
struct timeval inicioTempo;

int larg, alt, LARG, ALT;
int L, H; /* matriz de pixels */
int semente, NIT, T, v;
float epsilon, prob;
int nprocs;
float raio;

PONTO** lago;
PIXEL** imagem;

/*############ PROTOTIPOS ############# */ 
void parserEntrada(FILE* entrada, char* argv2);
void inicializaImagem();
void liberaGeral();
void inicializaPontosLago();
float tempoDesdeInicio(struct timeval inicio);
double calculaDistancia(PONTO p1, PONTO p2);
double calculaAltura(double r, double t);
void dimensoesMatrizLago();
int Euclides(int m, int n);

void imprimeMatrizLago();

/*############################################ */ 
 
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

	parserEntrada(entrada, argv[2]);
	dimensoesMatrizLago();
	inicializaPontosLago();
	inicializaImagem();

	imprimeMatrizLago();
	
	fclose(entrada);
	liberaGeral();

	return 0;
}


void inicializaImagem() {
	imagem = (PIXEL**) mallocSafe(H * sizeof(PIXEL*));
	int i, j;

	for (i = 0; i < H; i++) {
		imagem[i] = mallocSafe(L * sizeof(PIXEL));

		for (j = 0; j < L; j++)
			imagem[i][j] = (PIXEL){0, 0, 0, &lago[i][j]};
	}
}

void inicializaPontosLago() {
	lago = (PONTO**) mallocSafe(ALT * sizeof(PONTO*));
	int i, j, Cx, Cy;
	float x, y;

	Cy = 1;
	for (i = 0; i < ALT; i++) {
		lago[i] = mallocSafe(LARG * sizeof(PONTO));

		y = Cy * raio;
		Cx = 1;

		for (j = 0; j < LARG; j++) {			
			x = Cx * raio;
			Cx += 2;
			
			lago[i][j] = (PONTO){x, y, 0};
		}
		
		Cy += 2;
	}
}

void liberaGeral() {
	int i;

	for (i = 0; i < H; i++)
		free(imagem[i]);

	for (i = 0; i < ALT; i++)
		free(lago[i]);	
}

double calculaDistancia(PONTO p1, PONTO p2) {
	double d;

	d = pow (p1.x - p2.x, 2) + pow (p1.y - p2.y, 2);
	d = sqrt (d);

	return d;
}

double calculaAltura(double r, double t) {
	double h;

	h = r - v * t;
	h *= exp ((-1 * pow (r - v * t, 2)));
	h *= exp ((-t)/10);

	return h;
}

void parserEntrada(FILE* entrada, char* argv2) {
	fscanf(entrada,"(%d,%d)\n(%d,%d)\n%d\n%d\n%f\n%d\n%f\n%d\n", 
		&larg, &alt, &L, &H, &T, &v, &epsilon, &NIT, &prob, &semente);

	printf("\nlarg = %d\n", larg);
	printf("alt = %d\n", alt);
	printf("L = %d\n", L);
	printf("H = %d\n", H);
	printf("T = %d\n", T);
	printf("v = %d\n", v);
	printf("epsilon = %f\n", epsilon);
	printf("NIT = %d\n", NIT);
	printf("prob = %f\n", prob);
	printf("semente = %d\n", semente);
	
	nprocs = atoi(argv2);

	if (nprocs <= 0) { 
		nprocs = 1;
	}
}

void dimensoesMatrizLago() {
	float mdc = (float) Euclides(alt, larg);
	float diametro = 0.01 * mdc;

	LARG = (int)(larg/(diametro));  
	ALT = (int)(alt/(diametro));

	raio = diametro/2;  
}

/* MDC(m,n). Ref: http://www.ime.usp.br/~pf/algoritmos/aulas/recu.html */
int Euclides(int m, int n) {
   int r;
   do {
      r = m % n; 
      m = n; 
      n = r;
   } while (r != 0);
   return m; 
}

float tempoDesdeInicio(struct timeval inicio) {
	struct timeval fim;
	float timedif;
	gettimeofday(&fim, NULL);
	timedif = (float)(fim.tv_sec - inicio.tv_sec);
	timedif += (float)(fim.tv_usec - inicio.tv_usec)/1000000;
	return timedif;
}

void imprimeMatrizLago() {
	int i, j;

	printf("ALT = %d\n", ALT);
	printf("LARG = %d\n", LARG);

	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			printf("(%f, %f) ", lago[i][j].x, lago[i][j].y);
		}
		printf("\n");
	}
}
