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
#define FPS 60

#define RAIZ_DOS 1.4142

#define ARRIBA 1
#define ABAJO 2
#define IZQUIERDA 3
#define DERECHA 4

#define REBOTE 1
#define BLOQUEO 2
#define ELIMINAR 3

#define JUGADOR 1
#define PROYECTIL_JUGADOR 2
#define MANTICORA 3
#define PROYECTIL_MANTICORA 4
#define FENIX 5
#define PROYECTIL_FENIX 6

// Declaración de la estructura de una entidad, en la que se basan todos los personajes y proyectiles del juego
struct Entidad
{
    ALLEGRO_BITMAP *sprite;
    float x_pos;
    float y_pos;
    int vel;
    float vidas;
    float alto;
    float ancho;
    int mov_arr;
    int mov_aba;
    int mov_izq;
    int mov_der;
};

// Se encarga de inicializar una entidad, el primer argumento es la entidad que se quiere inicializar; el segundo representa qué tipo de entidad es; el tercero es un apuntador a otra entidad de referencia, en caso de ser necesario
void inicializar_entidad(struct Entidad *entidad, int tipo, struct Entidad *entidad_origen)
{
    switch (tipo)
    {
    case JUGADOR:
        entidad->x_pos = 0;
        entidad->y_pos = 0;
        entidad->vel = 0;
        entidad->vidas = 3;
        entidad->alto = al_get_bitmap_width(entidad->sprite)*1;
        entidad->ancho = al_get_bitmap_height(entidad->sprite)*1;
        entidad->mov_arr = 0;
        entidad->mov_aba = 0;
        entidad->mov_der = 0;
        entidad->mov_izq = 0;
        break;
    case PROYECTIL_JUGADOR:
        break;
    case MANTICORA:
        break;
    case PROYECTIL_MANTICORA:
        break;
    case FENIX:
        break;
    case PROYECTIL_FENIX:
        break;
    default:
        break;
    }
}

// Su tarea es dibujar una entidad dada en unas coordenadas determinadas
void dibujar_entidad(struct Entidad entidad)
{
    al_draw_scaled_bitmap(entidad.sprite, 0, 0, al_get_bitmap_width(entidad.sprite), al_get_bitmap_height(entidad.sprite), entidad.x_pos, entidad.y_pos, entidad.ancho, entidad.alto, 0);
}

// Se encarga de mover una entidad dada en cierta dirección, varias llamadas conjuntas a esta funión pueden resultar en movimientos diagonales
// Toma como argumentos 2 apuntadores de las coordenadas de la entidad, tambien delta (su velocidad) y la direccion en la que se debe mover dicha entidad
int mover_entidad(float *x_pos, float *y_pos, int delta, int direccion, int comportamiento)
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
    return 1;
}


int colisiona_AABB(struct Entidad entidad_uno, struct Entidad entidad_dos)
{
    int colisiona = 0;
    if (entidad_uno.x_pos < (entidad_dos.x_pos + entidad_dos.ancho) &&
        (entidad_uno.x_pos + entidad_uno.ancho) > entidad_dos.x_pos && 
        entidad_uno.y_pos < (entidad_dos.y_pos + entidad_dos.alto) &&
        (entidad_uno.y_pos + entidad_uno.alto) > entidad_dos.y_pos)
    {
        colisiona = 1;
    }
    return colisiona; 
}