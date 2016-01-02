#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
 
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
 
bool inicializar();
void varreFaixa(int linIn, int linF, int colIn, int colF);
void varreCirculo(float cx, float cy, float r);
 

#define FRACAO_FAIXA    0.70

const int LARGURA_TELA = 1024;
const int ALTURA_TELA = 768;
ALLEGRO_DISPLAY *janela = NULL;
ALLEGRO_BITMAP *bmp = NULL;

int main(int argc, char** argv) {
    float cx, cy, r;
    
    cx = LARGURA_TELA/2;
    cy = ALTURA_TELA/2;
    r = atof(argv[1]);

    if (!inicializar())
    {
        return -1;
    }
    
    bmp = al_create_bitmap(LARGURA_TELA, ALTURA_TELA);
    int h = al_get_bitmap_height(bmp);
    int w = al_get_bitmap_width(bmp);
    printf("height = %d\n", h);
    printf("width = %d\n\n", w);
        
    al_set_target_bitmap(bmp);
    al_clear_to_color(al_map_rgb(255,255,255));
    al_draw_circle(cx, cy, r, al_map_rgb(250, 0, 0), 1.0);
    
    varreCirculo(cx, cy, r);

    if(!al_save_bitmap("ondas.pcx", bmp))
        printf("Não salvou!\n"); 

    printf("TERMINOU.\n");
    return 0;
}

void varreCirculo(float cx, float cy, float r) {
    varreFaixa(floor(cy-FRACAO_FAIXA*r), ceil(cy+FRACAO_FAIXA*r), 
        floor(cx-r-2), ceil(cx-FRACAO_FAIXA*r));
    
    varreFaixa(floor(cy-r-2), ceil(cy-FRACAO_FAIXA*r), floor(cx-r), ceil(cx+r));
    varreFaixa(floor(cy+FRACAO_FAIXA*r), ceil(cy+r+2), floor(cx-r), ceil(cx+r));
    
    varreFaixa(floor(cy-FRACAO_FAIXA*r), ceil(cy+FRACAO_FAIXA*r), 
        floor(cx+FRACAO_FAIXA*r), ceil(cx+r+2));
}

void varreFaixa(int linIn, int linF, int colIn, int colF) {
    int y, x;
    unsigned char R;
    unsigned char G;
    unsigned char B;
    ALLEGRO_COLOR cor;

    for (y = linIn; y <= linF; y++) {
        for (x = colIn; x <= colF; x++) {
            cor = al_get_pixel(bmp, x, y);
            al_unmap_rgb(cor, &R, &G, &B);

            if (R == 250) {
                cor = al_map_rgb(0, 0, 0);
                al_put_pixel(x, y, cor);
            }
        }
    }
}
 
bool inicializar()
{
    if (!al_init())
    {
        fprintf(stderr, "Falha ao inicializar a biblioteca Allegro.\n");
        return false;
    }
 
    if (!al_init_primitives_addon())
    {
        fprintf(stderr, "Falha ao inicializar add-on de primitivas.\n");
        return false;
    }

    if (!al_init_image_addon())
    {
        fprintf(stderr, "Falha ao inicializar image_addon().\n");
        return false;
    }
    
    return true;
}