#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <sys/time.h>
#include "utils.h"
#include "linkedList.h"

typedef struct ponto {
	float x;
	float y; 
	float h;
	Link radiais;
} PONTO;  // no plano cartesiano

typedef struct pixel {
	int red;
	int green;
	int blue;
	PONTO* pto; // lago é mapeado pra imagem
} PIXEL;

/* ========================== VARIAVEIS GLOBAIS =========================== */
struct timeval inicioTempo;

int larg, alt; 
int LARG, ALT;
int L, H; /* matriz de pixels */
int semente, NIT, T, v;
float epsilon, prob;
int nprocs;
	
// DAQUI PARA FRENTE:
// RAIO e ANGULO sao relativos ao ponto de queda da gota
float draio;
float dtheta;
float hmax = 0, pmax = 0; // altura e profundidade max.

PONTO** lago;
PIXEL** imagem;

/* ========================== PROTOTIPOS =========================== */ 
void parserEntrada(FILE*, char*);
void inicializaImagem();
void liberaGeral();
void inicializaPontosLago();
float calculaAltura(float, float);
void dimensoesMatrizLago();
int temProximaGota();
PONTO sorteiaPosicaoGota();
PONTO polarEmCartesiano(float, float, PONTO*);

int pontoEstaDentro(PONTO*);
void atualizaPontoNoLago(PONTO*);
void propagaOndaCircular(PONTO*);
void propagaOndaPorNivel(float, float, PONTO*);

void imprimeMatrizLago();
void geraPPM(char* fname);
void defineCorPixels();
void TesteGota();

/*=================================================================== */ 
 
int main(int argc, char **argv) {
	gettimeofday(&inicioTempo, NULL);

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

	TesteGota();
	defineCorPixels();
	geraPPM("ondaTeste.ppm");

	fclose(entrada);
	liberaGeral();
	return 0;
}

void geraPPM(char* fname) {
	FILE* arq = fopen(fname, "w");
	int i, j;
	fprintf(arq, "P3\n");
	fprintf(arq, "%d %d\n", L, H);
	fprintf(arq, "255");

	for (i = 0; i < H; i++) {
		for (j = 0; j < L; j++) {
			fprintf(arq, "\n%d %d %d", imagem[i][j].red, imagem[i][j].green, imagem[i][j].blue);
		}
	}
	fclose(arq);
}

void defineCorPixels() {
	float delta, h;
	int i, j;

	pmax = (-1) * pmax;

	if (hmax >= pmax) 
		delta = hmax;
	else 
		delta = pmax;

	if (delta == 0)
		delta = 1;

	//delta = delta * (1/255);

	for (i = 0; i < H; i++) {
		for (j = 0; j < L; j++) {
			h = imagem[i][j].pto->h;
			
			if (h == 0) {
				imagem[i][j].red = imagem[i][j].green = imagem[i][j].blue = 255;
			}
			
			else if (h > 0) {
				imagem[i][j].red = imagem[i][j].green = 0;
				imagem[i][j].blue =	((int) ceil((double)(h/delta)));
				//imagem[i][j].blue =	0;
			}

			else {
				h *= (-1);
				imagem[i][j].blue = imagem[i][j].green = 0;
				imagem[i][j].red = ((int) ceil((double)(h/delta)));
				//imagem[i][j].red = 0;
			}
		}
	}
}

void TesteGota() {
	PONTO gota;
	gota = sorteiaPosicaoGota();
	//propagaOndaCircular(&gota);
	//propagaOndaPorNivel(0, 0, &gota);
	propagaOndaPorNivel(1, 0, &gota);
	propagaOndaPorNivel(2, 0, &gota);
	propagaOndaPorNivel(3, 1, &gota);
	propagaOndaPorNivel(5, 2, &gota);
	propagaOndaPorNivel(10, 5, &gota);
	propagaOndaPorNivel(15, 10, &gota);
	propagaOndaPorNivel(20, 20, &gota);
	propagaOndaPorNivel(30, 30, &gota);
	propagaOndaPorNivel(40, 10, &gota);
	propagaOndaPorNivel(50, 30, &gota);
}

float calculaAltura(float r, float t) {
	double h;
	/*
	double a, b, c;
	a = (double)r - (double)v * (double)t;
	b = pow(a, 2);
	b = exp((-1) * b);
	c = exp((-0.1) * t);
	h = a * b * c;
	*/

	h = r * t;

	return ((float)h);
}

void dimensoesMatrizLago() {
	ALT = 10 * alt;
	LARG = 10 * larg;
	draio = 0.050;
	dtheta = (float)(M_PI * 0.01);
	H = ALT;
	L = LARG;
	printf("ALT = %d\n", ALT);
	printf("LARG = %d\n", LARG);
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

void atualizaPontoNoLago(PONTO* foco) {
	int col = (int)(foco->x/(2*draio));
	int lin = (int)(foco->y/(2*draio));
	float modH;

	lago[lin][col].h += foco->h;
	modH = (float) fabs ((double)lago[lin][col].h);

	if (modH < epsilon)
		lago[lin][col].h = 0;

	if (lago[lin][col].h > hmax)
		hmax = lago[lin][col].h;

	if (lago[lin][col].h < pmax)
		pmax = lago[lin][col].h;
}


// THETA EM RADIANOS
PONTO polarEmCartesiano(float r, float theta, PONTO* origem) {
	PONTO p;

	float cosTh = (float) cos ((double)theta);
	float sinTh = (float) sin ((double)theta);
		
	p.x = origem->x + r * cosTh; 
	p.y = origem->y + r * sinTh; 

	return p;
}

int pontoEstaDentro(PONTO* p) {
	if (p->x <= 0 || p->x >= larg)
		return 0;
	if (p->y <= 0 || p->y >= alt)
		return 0;
	return 1;
}

void inicializaPontosLago() {
	int i, j, Cx, Cy;
	float x, y;
	lago = (PONTO**) mallocSafe(ALT * sizeof(PONTO*));

	Cy = 1;
	for (i = 0; i < ALT; i++) {
		lago[i] = mallocSafe(LARG * sizeof(PONTO));

		y = Cy * draio;
		Cx = 1;
		for (j = 0; j < LARG; j++) {			
			x = Cx * draio;
			Cx += 2;
			
			lago[i][j] = (PONTO){x, y, 0};
		}
		Cy += 2;
	}
}

int temProximaGota() {
	Randomize(semente);

	if (RandomReal(0, 1) < 0.01 * prob) return 1;
	return 0;
}

PONTO sorteiaPosicaoGota() {
	PONTO novo;

	Randomize(semente);
	novo.x = RandomReal(draio, larg - draio);
	novo.y = RandomReal(draio, alt - draio);

	return novo;
}

void propagaOndaCircular(PONTO* centro) {
	int totRadiais = 2 * (int)(M_PI/dtheta);
	struct timeval tempoGota;
	float raio, intervaloRadial = (float)(draio/v);

	centro->radiais = initialize();
	centro->h = calculaAltura(0, 0); 
	
	//centro foi sorteado, não está na matriz lago
	atualizaPontoNoLago(centro);
	gettimeofday(&tempoGota, NULL); //inicia contagem do tempo desde a queda

	for (raio = draio; centro->radiais->total < totRadiais; raio += draio) {
		
		// precisa dar tempo de chegar no proximo raio
		while (tempoDesdeInicio(tempoGota) < intervaloRadial)
			usleep(100000); // 100 ms

		propagaOndaPorNivel(raio, tempoDesdeInicio(tempoGota), centro);
	}

	freeAll(centro->radiais);
}

void liberaGeral() {
	int i;
	for (i = 0; i < ALT; i++)
		free(lago[i]);

	for (i = 0; i < H; i++)
		free(imagem[i]);	
}

void imprimeMatrizLago() {
	int i, j;

	for (i = 0; i < ALT; i++) {
		for (j = 0; j < LARG; j++) {
			printf("(%f, %f) ", lago[i][j].x, lago[i][j].y);
		}
		printf("\n");
	}
}

void inicializaImagem() {
	imagem = (PIXEL**) mallocSafe(H * sizeof(PIXEL*));
	int i, j;

	for (i = 0; i < H; i++) {
		imagem[i] = mallocSafe(L * sizeof(PIXEL));

		for (j = 0; j < L; j++)
			imagem[i][j] = (PIXEL){255, 255, 255, &lago[i][j]};
	}
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