#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include "estatistics.h"
#include "globals.h"
#include "linkedList.h"

static double average(Link head);
static double standardDeviation(Link head, double media);

static double average(Link head) {
    double media = 0.0;
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

static double standardDeviation(Link head, double media) {
    double dp = 0.0;
    double dif;
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

void geraArquivoEstatistico(char* fname) {
    FILE* arq = fopen(fname, "w");
    int i, j;
    double media, dp;

    fprintf(arq, "pos.x | pos.y | media | desvio_padrao\n\n");

    for (i = 0; i < H; i++) 
    {
        #pragma omp parallel for private(media, dp) 
            for (j = 0; j < L; j++) 
            {
                media = 0;
                dp = 0;
                if (node[i][j].hist->total) {
                    media = average(node[i][j].hist);
                    dp = standardDeviation(node[i][j].hist, media);
                    
                    if (media != 0) {
                    
                       fprintf(arq, "%12.7f | %12.7f | %12.7f | %12.7f\n", 
                                node[i][j].pto.x, node[i][j].pto.y, media, dp); 
                        
                    }
                }       
            }
    }
    fclose(arq);
}