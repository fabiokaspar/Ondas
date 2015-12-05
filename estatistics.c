#include <math.h>
#include <stdlib.h>
#include "estatistics.h"

float average(Link head) {
    float media = 0.0;
    int N = head->total;
    Link lk;

    if (N == 0)
        return 0.0;

    for (lk = head->next; lk != NULL; lk = lk->next) {
      media += lk->altura;
    }

    media /= N;

    return media;
}

float standardDeviation(Link head, float media) {
    float dp = 0.0;
    float dif;
    int N = head->total;
    Link lk;

    if (N == 0)
        return 0.0;

    for (lk = head->next; lk != NULL; lk = lk->next) {
        dif = lk->altura - media;
        dp += (dif * dif);
    }

    dp /= N;

    dp = sqrt(dp);

    return dp;
}