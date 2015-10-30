#include <stdlib.h>
#include <stdio.h>

int larg, alt;
int L, H; /* matriz de pixels */
int semente, NIT, T, v; 
float epsilon, prob; 

FILE* entrada;
int nprocs;

void parserEntrada(FILE* entrada);

int main(int argc, char **argv) {	
	if (argc < 3) { 
		fprintf(stderr, "formato exigido: ./ep <arquivo> <#processadores>\n");
		return 0; 
	}

	entrada = fopen(argv[1], "r");
	if (!entrada) {
		fprintf(stderr, "ERRO ao abrir arquivo %s .\n", argv[1]); 
		return 0;
	}

	parserEntrada(entrada);


	nprocs = atoi(argv[2]);
	if (!nprocs) { 
		nprocs = 2;
	}

	fclose(entrada);

	return 0;
}

void parserEntrada(FILE* arq) {
	fscanf(arq,"(%d,%d)\n(%d,%d)\n%d\n%d\n%f\n%d\n%f\n%d\n", 
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
}

