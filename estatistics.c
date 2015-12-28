#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include "estatistics.h"
#include "globals.h"

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

void geraArquivoEstatistico() {
    FILE* arq = fopen("estatistica.txt", "w");
    int i, j;
    float media, dp;

    fprintf(arq, "pos.x | pos.y | media | desvio_padrao\n\n");

    for (i = 0; i < H; i++) 
    {
        #pragma omp parallel for private(media, dp) 
            for (j = 0; j < L; j++) 
            {
                media = 0;
                dp = 0;
               // if (lago[i][j].alturas->total) {
                    //media = average(lago[i][j].alturas);
                    //dp = standardDeviation(lago[i][j].alturas, media);
                    
                    if (dp != 0 || media != 0) {
                        #pragma omp critical
                        {   fprintf(arq, "%12.7f | %12.7f | %12.7f | %12.7f\n", 
                                lago[i][j].pto.x, lago[i][j].pto.y, media, dp); 
                        }
                    }
              //  }       
            }
    }
    printf("estatistica.txt gerado com sucesso\n");
    fclose(arq);
}