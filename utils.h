#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>

void* mallocSafe(size_t bytes);
int RandomInteger(int low, int high);
double RandomReal (double low, double high);
void Randomize(int seed);
float tempoDesdeInicio(struct timeval inicio);
void dorme(float tSeg);

#endif