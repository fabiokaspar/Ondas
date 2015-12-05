#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include "utils.h"

// http://www.ime.usp.br/~pf/algoritmos/aulas/aloca.html
void* mallocSafe(size_t bytes) {
	void* p = malloc(bytes);
	
	if (p != NULL) 
		return p;
	
	else { 
		fprintf(stderr, "Memoria insuficiente!\n");
		exit(0);
	}
	return NULL;
}

// A função RandomInteger devolve um inteiro 
// aleatório entre low e high inclusive,
// ou seja, no intervalo fechado low..high.
// Vamos supor que low <= high e que 
// high - low + 1 <= RAND_MAX + 1.
// (O código foi copiado da biblioteca random
// de Eric Roberts.) 
// https://linux.ime.usp.br/~lucasmmg/livecd/documentacao/documentos/mac122/random.html

int RandomInteger(int low, int high)
{
    int k;
    double d;
    d = (double) rand () / ((double) RAND_MAX + 1);
    k = d * (high - low + 1);
    return low + k;
}


// A função RandomReal devolve um número real 
// (pseudo)aleatório no intervalo
// [low, high]. O código foi adaptado da biblioteca
// random de Eric Roberts.
// https://linux.ime.usp.br/~lucasmmg/livecd/documentacao/documentos/mac122/random.html

double RandomReal (double low, double high)
{
    double d;

    d = (double) rand () / ((double) RAND_MAX);
    return low + d * (high - low);
}


//  Ref: http://www.ime.usp.br/~pf/algoritmos/aulas/random.html
void Randomize(int seed)
{
    srand(seed + time (NULL));
}

float tempoDesdeInicio(struct timeval inicio) {
	struct timeval fim;
	float timedif;
	gettimeofday(&fim, NULL);
	timedif = (float)(fim.tv_sec - inicio.tv_sec);
	timedif += (float)(fim.tv_usec - inicio.tv_usec)/1000000;
	return timedif;
}

void dorme(float tSeg) {
	long long int v = (int)tSeg * 1000000;
	usleep(v);
}