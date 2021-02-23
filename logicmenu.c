#include <stdio.h>
#include <ctype.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

int ops = 1;
int color_fuente = 0;
ALLEGRO_COLOR color_select;
ALLEGRO_COLOR color_evento(int ops, int prue)
{
    if (ops == prue)
    {
        color_select= al_map_rgb(212, 197, 32);
    }
    else
        color_select= al_map_rgb(212, 122, 32);

    return color_select;
}
void dibujar_fuego (struct Entidad *entidad, ALLEGRO_BITMAP *imagen1, ALLEGRO_BITMAP *imagen2, ALLEGRO_BITMAP *imagen3)
{
    if(entidad->sprite == imagen1)
    {
        entidad->sprite = imagen2;
    }
    else if (entidad->sprite == imagen2)
    {
        entidad->sprite = imagen3;
    }
    else 
        entidad->sprite = imagen1;
}