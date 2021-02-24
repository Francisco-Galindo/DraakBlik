#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
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
    int vidas;
    float alto;
    float ancho;
    int mov_arr;
    int mov_aba;
    int mov_izq;
    int mov_der;
};


enum IMAGENES {JUGADOR_IMAGEN, ICONO_IMAGEN, WALL_IMAGEN, MONT1_IMAGEN, MONT2_IMAGEN, LUNA_IMAGEN, DRAGO_LUNA_IMAGEN, FUEGO_0_IMAGEN, FUEGO_1_IMAGEN, FUEGO_2_IMAGEN, FONDO_0_IMAGEN, FONDO_1_IMAGEN, FONDO_2_IMAGEN, FONDO_3_IMAGEN, TORRE_0_IMAGEN, TORRE_1_IMAGEN};

enum FUENTES {FUENTE_60, FUENTE_40, FUENTE_20, FUENTE_15, FUENTE_TITULO_80, FUENTE_TITULO_50};

ALLEGRO_BITMAP *imagenes[20];
ALLEGRO_FONT *fuentes[5];


void cargar_imagenes(int *fin)
{
    imagenes[JUGADOR_IMAGEN] = al_load_bitmap("Imagenes/DRAV.png");
    imagenes[ICONO_IMAGEN] = al_load_bitmap("Imagenes/item.png");
    imagenes[WALL_IMAGEN] = al_load_bitmap("Imagenes/fondo.png");
    imagenes[MONT1_IMAGEN] = al_load_bitmap("Imagenes/mnt5.png");
    imagenes[MONT2_IMAGEN] = al_load_bitmap("Imagenes/mnt6.png");
    imagenes[LUNA_IMAGEN] = al_load_bitmap("Imagenes/luna.png");
    imagenes[DRAGO_LUNA_IMAGEN] = al_load_bitmap("Imagenes/dragolun.png");
    imagenes[FUEGO_0_IMAGEN] = al_load_bitmap("Imagenes/fuego.png");
    imagenes[FUEGO_1_IMAGEN] = al_load_bitmap("Imagenes/fuego1.png");
    imagenes[FUEGO_2_IMAGEN] = al_load_bitmap("Imagenes/fuego2.png");
    imagenes[FONDO_0_IMAGEN] = al_load_bitmap("Imagenes/mnt8.png");
    imagenes[FONDO_1_IMAGEN] = al_load_bitmap("Imagenes/mnt7.png");
    imagenes[FONDO_2_IMAGEN] = al_load_bitmap("Imagenes/mnt9.png");
    imagenes[FONDO_3_IMAGEN] = al_load_bitmap("Imagenes/mnt10.png");
    imagenes[TORRE_0_IMAGEN] = al_load_bitmap("Imagenes/torre3.png");
    imagenes[TORRE_1_IMAGEN] = al_load_bitmap("Imagenes/torre4.png");
    for (int i = 0; i < TORRE_1_IMAGEN; i++)
    {
        if (!imagenes[i])
        {
            printf("No se ha podido cargar la imagen %i", i);
            *fin =  1;
        }
    }
}


void cargar_fuentes(int *fin)
{
    fuentes[FUENTE_60] = al_load_font("Fuentes/PressStart2P.ttf", 60, 0);
    fuentes[FUENTE_40] = al_load_font("Fuentes/PressStart2P.ttf", 40, 0);
    fuentes[FUENTE_20] = al_load_font("Fuentes/PressStart2P.ttf", 20, 0);
    fuentes[FUENTE_15] = al_load_font("Fuentes/PressStart2P.ttf", 15, 0);
    fuentes[FUENTE_TITULO_80] = al_load_font("Fuentes/PerfectDOSVGA437.ttf", 80, 0);
    fuentes[FUENTE_TITULO_50] = al_load_font("Fuentes/PerfectDOSVGA437.ttf", 50, 0);
    for (int i = 0; i < FUENTE_TITULO_50; i++)
    {
        if (!fuentes[i])
        {
            printf("No se ha podido cargar la fuente %i\n", i);
            *fin =  1;
        }
    }
}


// Se encarga de inicializar una entidad, el primer argumento es la entidad que se quiere inicializar; el segundo representa qué tipo de entidad es; el tercero es un apuntador a otra entidad de referencia, en caso de ser necesario
void inicializar_entidad(struct Entidad *entidad, int tipo, struct Entidad *entidad_origen, ALLEGRO_BITMAP *imagen)
{
    entidad->mov_arr = 0;
    entidad->mov_aba = 0;
    entidad->mov_der = 0;
    entidad->mov_izq = 0;
    switch (tipo)
    {
        case JUGADOR:
            entidad->sprite = imagenes[JUGADOR_IMAGEN];
            entidad->vel = 0;
            entidad->vidas = 3;
            entidad->alto = al_get_bitmap_height(entidad->sprite)*1;
            entidad->ancho = al_get_bitmap_width(entidad->sprite)*1;
            entidad->x_pos = 0;
            entidad->y_pos = ALTO/2 - (entidad->alto / 2);
            break;
        case PROYECTIL_JUGADOR:
            break;
        case MANTICORA:
            entidad->sprite = imagenes[JUGADOR_IMAGEN];
            entidad->x_pos = ANCHO-100;
            entidad->y_pos = rand()%ALTO;
            entidad->vel = 0;
            entidad->vidas = 1;
            entidad->alto = al_get_bitmap_height(entidad->sprite)*1;
            entidad->ancho = al_get_bitmap_width(entidad->sprite)*1;
            break;
        case PROYECTIL_MANTICORA:
            break;
        case FENIX:
            break;
        case PROYECTIL_FENIX:
            break;
        default:
            entidad->sprite = imagen;
            entidad->alto = al_get_bitmap_height(entidad->sprite)*1;
            entidad->ancho = al_get_bitmap_width(entidad->sprite)*1;
            break;
    }
}


// La subrutina toma como parámetro el arreglo de todas las entidades, junto con el tipo de entidad que se quiere crear, y una entidad de origen en caso de ser necesaria. Se encarga de añadir dicha entidad al arreglo y aumentar el contador de entidades
void crear_entidad(struct Entidad entidades[], int *contador, int tipo, struct Entidad *entidad_origen, ALLEGRO_BITMAP *imagen)
{
    int count = *contador;
    inicializar_entidad(&entidades[count], tipo, entidad_origen, imagen);
    *contador = count + 1;
}


// Esta subrutina se usa para poder "eliminar" una entidad del arreglo de entidades, esto se logra moviendo un lugar las entidades que vengan después de aquella entidad que se quiere eliminar.
void eliminar_entidad(struct Entidad entidades[], int indice, int *contador)
{
    int count = *contador;
    for (int i = indice; i < count; i++)
        entidades[i] = entidades[i+1];
    *contador = count - 1;
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

// Función que checa si una entidad colisiona con otra, se usa el algoritmo AABB.
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


void inicializar_modo(struct Entidad entidades[], int modo, int *contador, struct Entidad *jugador)
{
    switch (modo)
    {
    case 0:
        crear_entidad(entidades, contador, -1, NULL, imagenes[FUEGO_0_IMAGEN]);
        crear_entidad(entidades, contador, -1, NULL, imagenes[FUEGO_0_IMAGEN]);
        entidades[0].ancho = al_get_bitmap_width(entidades[0].sprite)*0.15;
        entidades[0].alto = al_get_bitmap_height(entidades[0].sprite)*0.15;
        entidades[0].x_pos = 15;
        entidades[0].y_pos = 100;
        entidades[1].ancho = al_get_bitmap_width(entidades[1].sprite)*0.15;
        entidades[1].alto = al_get_bitmap_height(entidades[1].sprite)*0.15;
        entidades[1].x_pos = 480;
        entidades[1].y_pos = 105;
        break;
    case 1:
        inicializar_entidad(jugador, JUGADOR, NULL, NULL);
        break;
    default:
        break;
    }
}