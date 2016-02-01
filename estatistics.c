#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include "estatistics.h"
#include "globals.h"
#include "linkedList.h"

static double Media(Link head);
static double DesvioPadrao(Link head, double media);

static double Media(Link head) {
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

static double DesvioPadrao(Link head, double media) {
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

    fprintf(arq, "pos.x | pos.y | media | desvio padrão\n\n");

    for (i = 0; i < H; i++)
    {
        #pragma omp parallel for private(media, dp) schedule(dynamic) ordered
            for (j = 0; j < L; j++)
            {
                media = 0;
                dp = 0;
                if (node[i][j].hist->total > 0) {
                    media = Media(node[i][j].hist);
                    dp = DesvioPadrao(node[i][j].hist, media);
                    
                    #pragma omp ordered
                    {
                        fprintf(arq, "%12.7f | %12.7f | %12.7f | %12.7f\n", 
                            node[i][j].pto.x, node[i][j].pto.y, media, dp); 
                    }
                }       
            }
    }
    fclose(arq);
}