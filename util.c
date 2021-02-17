#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#define ALTO 480
#define ANCHO 640
#define VEL 14
#define FPS 30

#define ARRIBA 1
#define ABAJO 2
#define IZQUIERDA 3
#define DERECHA 4

#define REBOTE 1
#define BLOQUEO 2
#define ELIMINAR 3

struct Jugador
{
    ALLEGRO_BITMAP *sprite;
    float x_pos;
    float y_pos;
    int vel;
    int mov_arr;
    int mov_aba;
    int mov_izq;
    int mov_der;
};

void inicializar_jugador(struct Jugador jugador)
{
    jugador.x_pos = 0;
    jugador.y_pos = 0;
    jugador.vel = 0;
    jugador.mov_arr = 0;
    jugador.mov_aba = 0;
    jugador.mov_der = 0;
    jugador.mov_izq =0;
}

void dibujar_jugador(struct Jugador jugador, float x, float y)
{
    al_draw_scaled_bitmap(jugador.sprite, 0, 0, al_get_bitmap_width(jugador.sprite), al_get_bitmap_height(jugador.sprite), x, y, al_get_bitmap_width(jugador.sprite)*1, al_get_bitmap_height(jugador.sprite)*1, 0);
}

void mover_entidad(float *x_pos, float *y_pos, int delta, int direccion, int comportamiento)
{
    switch (direccion)
    {
    case ARRIBA:
        *y_pos -= delta;
        if (*y_pos < -10)
            *y_pos = -10;
        break;
    case ABAJO:
        *y_pos += delta;
        if (*y_pos > ALTO-54)
            *y_pos = ALTO-54; 
        break;
    case IZQUIERDA:
        *x_pos -= delta;
        if (*x_pos < -10)
            *x_pos = -10; 
        break;
    case DERECHA:
        *x_pos += delta;
        if (*x_pos > ANCHO-54)
            *x_pos = ANCHO-54; 
        break;
    }
}