#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>

void* mallocSafe(size_t bytes);
int RandomInteger(int low, int high);
double RandomReal (double low, double high);
void Randomize(int seed);
int Euclides(int m, int n);
float tempoDesdeInicio(struct timeval inicio);

#endif