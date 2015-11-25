#include <stdio.h>
#include <time.h>
#include "utils.h"

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
// de Eric Roberts.) Ref: http://www.ime.usp.br/~pf/algoritmos/aulas/random.html
int RandomInteger(int low, int high)
{
    int k;
    double d;
    d = (double) rand () / ((double) RAND_MAX + 1);
    k = d * (high - low + 1);
    return low + k;
}

//  Ref: http://www.ime.usp.br/~pf/algoritmos/aulas/random.html
void Randomize(int seed)
{
    srand(seed + time (NULL));
}
