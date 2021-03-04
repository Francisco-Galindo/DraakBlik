#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#define ALTO 480
#define ANCHO 640
#define FPS 60
#define VEL 600

#define PI 3.141592

#define RAIZ_DOS 1.4142

#define ARRIBA 1
#define ABAJO 2
#define IZQUIERDA 3
#define DERECHA 4

#define REBOTE 1
#define BLOQUEO 2
#define ELIMINAR 3

#define JUGADOR 1
#define MANTICORA 2
#define FENIX 3
#define GARGOLA 4
#define HYDRA 5
#define FUEGO 6
#define PROYECTIL_JUGADOR 7
#define PROYECTIL_MANTICORA 8
#define PROYECTIL_FENIX 9
#define PROYECTIL_GARGOLA 10
#define PROYECTIL_HYDRA 11
#define GENERICO 12

// Declaración de la estructura de una entidad, en la que se basan todos los personajes y proyectiles del juego
struct Entidad
{
    int tipo;
    ALLEGRO_BITMAP *sprite;
    float x_pos;
    float y_pos;
    float max_vel;
    float x_vel;
    float y_vel;
    int vidas;
    float alto;
    float ancho;
};

struct Usuario
{
    char nombre[16];
    int puntaje;
};


enum imagenes {JUGADOR_IMAGEN_0, JUGADOR_IMAGEN_1, ICONO_IMAGEN, WALL_IMAGEN, MONT1_IMAGEN, MONT2_IMAGEN, LUNA_IMAGEN, DRAGO_LUNA_IMAGEN, FUEGO_0_IMAGEN, FUEGO_1_IMAGEN, FUEGO_2_IMAGEN, FONDO_0_IMAGEN, FONDO_1_IMAGEN, FONDO_2_IMAGEN, FONDO_3_IMAGEN, TORRE_0_IMAGEN, TORRE_1_IMAGEN, EXPLOSION_IMAGEN, MANTICORA_IMAGEN_0, MANTICORA_IMAGEN_1, FENIX_IMAGEN_0, FENIX_IMAGEN_1, HYDRA_IMAGEN_0, HYDRA_IMAGEN_1, GARGOLA_IMAGEN_0, GARGOLA_IMAGEN_1, PROYECTIL_0_IMAGEN, PROYECTIL_1_IMAGEN, PROYECTIL_2_IMAGEN, PROYECTIL_3_IMAGEN, PROYECTIL_4_IMAGEN};

enum fuentes {FUENTE_60, FUENTE_40, FUENTE_20, FUENTE_15,FUENTE_TITULO_20, FUENTE_TITULO_80, FUENTE_TITULO_50};

enum sonidos {MENU_TEMA, CREDITOS_TEMA, JUEGO_TEMA, SELECCION_SONIDO, DANO_SONIDO, DISPARO_SONIDO};

ALLEGRO_BITMAP *imagenes[31];
ALLEGRO_FONT *fuentes[7];
ALLEGRO_SAMPLE *sonidos[11];


void imagenes_cargar(int *fin)
{
    imagenes[JUGADOR_IMAGEN_0] = al_load_bitmap("Imagenes/DRAV.png");
    imagenes[JUGADOR_IMAGEN_1] = al_load_bitmap("Imagenes/DRAV_ abajo.png");
    imagenes[ICONO_IMAGEN] = al_load_bitmap("Imagenes/item.png");
    imagenes[WALL_IMAGEN] = al_load_bitmap("Imagenes/fondo.png");
    imagenes[MONT1_IMAGEN] = al_load_bitmap("Imagenes/mnt_5.png");
    imagenes[MONT2_IMAGEN] = al_load_bitmap("Imagenes/mnt_6.png");
    imagenes[LUNA_IMAGEN] = al_load_bitmap("Imagenes/luna.png");
    imagenes[DRAGO_LUNA_IMAGEN] = al_load_bitmap("Imagenes/dragolun.png");
    imagenes[FUEGO_0_IMAGEN] = al_load_bitmap("Imagenes/fuego.png");
    imagenes[FUEGO_1_IMAGEN] = al_load_bitmap("Imagenes/fuego1.png");
    imagenes[FUEGO_2_IMAGEN] = al_load_bitmap("Imagenes/fuego2.png");
    imagenes[FONDO_0_IMAGEN] = al_load_bitmap("Imagenes/mnt_8.png");
    imagenes[FONDO_1_IMAGEN] = al_load_bitmap("Imagenes/mnt_7.png");
    imagenes[FONDO_2_IMAGEN] = al_load_bitmap("Imagenes/mnt_9.png");
    imagenes[FONDO_3_IMAGEN] = al_load_bitmap("Imagenes/mnt_10.png");
    imagenes[TORRE_0_IMAGEN] = al_load_bitmap("Imagenes/torre3.png");
    imagenes[TORRE_1_IMAGEN] = al_load_bitmap("Imagenes/torre4.png");
    imagenes[EXPLOSION_IMAGEN] = al_load_bitmap("Imagenes/Explosion.png");
    imagenes[MANTICORA_IMAGEN_0] = al_load_bitmap("Imagenes/quimera.png");
    imagenes[MANTICORA_IMAGEN_1] = al_load_bitmap("Imagenes/quimera_abajo.png");
    imagenes[FENIX_IMAGEN_0] = al_load_bitmap("Imagenes/fenix_izquierda.png");
    imagenes[FENIX_IMAGEN_1] = al_load_bitmap("Imagenes/fenix.png");
    imagenes[HYDRA_IMAGEN_0] = al_load_bitmap("Imagenes/HYDRA.png");
    imagenes[HYDRA_IMAGEN_1] = al_load_bitmap("Imagenes/HYDRA_abajo.png");
    imagenes[GARGOLA_IMAGEN_0] = al_load_bitmap("Imagenes/gargola_arriba.png");
    imagenes[GARGOLA_IMAGEN_1] = al_load_bitmap("Imagenes/gargola_abajo.png");
    imagenes[PROYECTIL_0_IMAGEN] = al_load_bitmap("Imagenes/disparo_dragon.png");
    imagenes[PROYECTIL_1_IMAGEN] = al_load_bitmap("Imagenes/disparo_fenix.png");
    imagenes[PROYECTIL_2_IMAGEN] = al_load_bitmap("Imagenes/disparo_hydra.png");
    imagenes[PROYECTIL_3_IMAGEN] = al_load_bitmap("Imagenes/disparo_gargola.png");
    imagenes[PROYECTIL_4_IMAGEN] = al_load_bitmap("Imagenes/disparo_quimera.png");

    for (int i = 0; i < PROYECTIL_3_IMAGEN; i++)
    {
        if (!imagenes[i])
        {
            printf("No se ha podido cargar la imagen %i", i);
            *fin =  1;
        }
    }
}


void fuentes_cargar(int *fin)
{
    fuentes[FUENTE_60] = al_load_font("Fuentes/PressStart2P.ttf", 60, 0);
    fuentes[FUENTE_40] = al_load_font("Fuentes/PressStart2P.ttf", 40, 0);
    fuentes[FUENTE_20] = al_load_font("Fuentes/PressStart2P.ttf", 20, 0);
    fuentes[FUENTE_15] = al_load_font("Fuentes/PressStart2P.ttf", 15, 0);
    fuentes[FUENTE_TITULO_20] = al_load_font("Fuentes/PerfectDOSVGA437.ttf", 20, 0);
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

void sonidos_cargar(int *fin)
{
    sonidos[MENU_TEMA] = al_load_sample("Audio/soundtrack_menu.mp3");
    sonidos[CREDITOS_TEMA] = al_load_sample("Audio/soundtrack_creditos.mp3");
    sonidos[JUEGO_TEMA] = al_load_sample("Audio/soundtrack_juego.mp3");
    sonidos[SELECCION_SONIDO] = al_load_sample("Audio/select.wav");
    sonidos[DANO_SONIDO] = al_load_sample("Audio/dano.wav");
    sonidos[DISPARO_SONIDO] = al_load_sample("Audio/disparo_sonido.wav");
    for (int i = 0; i < DISPARO_SONIDO; i++)
    {
        if(!sonidos[i])
        {
            printf("No se ha podido cargar el sonido %i\n", i);
            *fin =  1;
        }
    }
}


// Se encarga de inicializar una entidad, el primer argumento es la entidad que se quiere inicializar; el segundo representa qué tipo de entidad es; el tercero es un apuntador a otra entidad de referencia, en caso de ser necesario
void entidad_inicializar(struct Entidad *entidad, int tipo, struct Entidad *entidad_origen, ALLEGRO_BITMAP *imagen)
{
    entidad->x_pos = 0;
    entidad->y_pos = 0;
    entidad->x_vel = 0;
    entidad->y_vel = 0;
    entidad->tipo = tipo;
    switch (tipo)
    {
        case JUGADOR:
            entidad->sprite = imagenes[JUGADOR_IMAGEN_0];
            entidad->max_vel = VEL/FPS;
            entidad->vidas = 5;
            entidad->alto = al_get_bitmap_height(entidad->sprite)*1;
            entidad->ancho = al_get_bitmap_width(entidad->sprite)*1;
            entidad->x_pos = 0;
            entidad->y_pos = ALTO/2 - (entidad->alto / 2);
            break;
        case PROYECTIL_JUGADOR:
            entidad->sprite = imagenes[PROYECTIL_0_IMAGEN];
            entidad->max_vel = (VEL/FPS) * 2;
            entidad->x_vel = entidad->max_vel;
            entidad->vidas = 1;
            entidad->alto = al_get_bitmap_height(entidad->sprite)*0.125;
            entidad->ancho = al_get_bitmap_width(entidad->sprite)*0.125;
            entidad->x_pos = entidad_origen->x_pos + (entidad_origen->ancho / 2);
            entidad->y_pos = entidad_origen->y_pos + (entidad_origen->alto / 2);
            break;
        case MANTICORA:
            entidad->sprite = imagenes[MANTICORA_IMAGEN_0];
            entidad->x_pos = ANCHO-100;
            entidad->y_pos = rand()%ALTO;
            entidad->max_vel = (VEL/FPS)/2.0;
            entidad->vidas = 1;
            entidad->alto = al_get_bitmap_height(entidad->sprite)*1.25;
            entidad->ancho = al_get_bitmap_width(entidad->sprite)*1.25;
            entidad->x_vel = entidad->max_vel/2;
            entidad->y_vel = entidad->max_vel/2;
            break;
        case PROYECTIL_MANTICORA:
            entidad->sprite = imagenes[PROYECTIL_4_IMAGEN];
            entidad->max_vel = ((VEL/FPS));
            entidad->x_vel = entidad->max_vel;
            entidad->vidas = 1;
            entidad->alto = al_get_bitmap_height(entidad->sprite)*0.125;
            entidad->ancho = al_get_bitmap_width(entidad->sprite)*0.125;
            entidad->x_pos = entidad_origen->x_pos + (entidad_origen->ancho / 2);
            entidad->y_pos = entidad_origen->y_pos + (entidad_origen->alto / 2);
            break;
        case FENIX:
            entidad->sprite = imagenes[FENIX_IMAGEN_0];
            entidad->x_pos = ANCHO-100;
            entidad->y_pos = rand()%ALTO;
            entidad->max_vel = (VEL/FPS)/2;
            entidad->vidas = 1;
            entidad->alto = al_get_bitmap_height(entidad->sprite)*0.1;
            entidad->ancho = al_get_bitmap_width(entidad->sprite)*0.1;
            break;
        case PROYECTIL_FENIX:
            entidad->sprite = imagenes[PROYECTIL_1_IMAGEN];
            entidad->max_vel = (VEL/FPS);
            entidad->x_vel = entidad->max_vel;
            entidad->vidas = 1;
            entidad->alto = al_get_bitmap_height(entidad->sprite)*0.125;
            entidad->ancho = al_get_bitmap_width(entidad->sprite)*0.125;
            entidad->x_pos = entidad_origen->x_pos + (entidad_origen->ancho / 2);
            entidad->y_pos = entidad_origen->y_pos + (entidad_origen->alto / 2);
            break;
        case GARGOLA:
            entidad->sprite = imagenes[GARGOLA_IMAGEN_0];
            entidad->x_pos = ANCHO-100;
            entidad->y_pos = rand()%ALTO;
            entidad->max_vel = (VEL/FPS)/3;
            entidad->vidas = 1;
            entidad->alto = al_get_bitmap_height(entidad->sprite)*1;
            entidad->ancho = al_get_bitmap_width(entidad->sprite)*1;
            break;
        case PROYECTIL_GARGOLA:
            entidad->sprite = imagenes[PROYECTIL_3_IMAGEN];
            entidad->max_vel = (VEL/FPS) * -1;
            entidad->x_vel = entidad->max_vel;
            entidad->vidas = 1;
            entidad->alto = al_get_bitmap_height(entidad->sprite)*0.125;
            entidad->ancho = al_get_bitmap_width(entidad->sprite)*0.125;
            entidad->x_pos = entidad_origen->x_pos + (entidad_origen->ancho / 2);
            entidad->y_pos = entidad_origen->y_pos + (entidad_origen->alto / 2);
            break;
        case HYDRA:
            entidad->sprite = imagenes[HYDRA_IMAGEN_0];
            entidad->x_pos = ANCHO-160;
            entidad->max_vel = (VEL/FPS)/3;
            entidad->vidas = 1;
            entidad->alto = al_get_bitmap_height(entidad->sprite)*1;
            entidad->ancho = al_get_bitmap_width(entidad->sprite)*1;
            entidad->y_pos = 0;
            break;
        case PROYECTIL_HYDRA:
            entidad->sprite = imagenes[PROYECTIL_2_IMAGEN];
            entidad->max_vel = ((VEL/FPS) * 2) / 2;
            entidad->x_vel = entidad->max_vel;
            entidad->vidas = 1;
            entidad->alto = al_get_bitmap_height(entidad->sprite)*0.125;
            entidad->ancho = al_get_bitmap_width(entidad->sprite)*0.125;
            entidad->x_pos = entidad_origen->x_pos + (entidad_origen->ancho / 2);
            entidad->y_pos = entidad_origen->y_pos + (entidad_origen->alto / 2);
            break;
        case FUEGO:
            entidad->sprite = imagenes[FUEGO_0_IMAGEN];
            entidad->max_vel = 0;
            entidad->alto = al_get_bitmap_height(entidad->sprite)*0.15;
            entidad->ancho = al_get_bitmap_width(entidad->sprite)*0.15;
            break;
        default:
            entidad->tipo = GENERICO; 
            entidad->sprite = imagen;
            entidad->alto = al_get_bitmap_height(entidad->sprite);
            entidad->ancho = al_get_bitmap_width(entidad->sprite);
            break;
    }
}


// La subrutina toma como parámetro el arreglo de todas las entidades, junto con el tipo de entidad que se quiere crear, y una entidad de origen en caso de ser necesaria. Se encarga de añadir dicha entidad al arreglo y aumentar el contador de entidades
void entidad_crear(struct Entidad entidades[], int *contador, int tipo, struct Entidad *entidad_origen, ALLEGRO_BITMAP *imagen)
{
    int count = *contador;
    entidad_inicializar(&entidades[count], tipo, entidad_origen, imagen);
    *contador = count + 1;
}


// Esta subrutina se usa para poder "eliminar" una entidad del arreglo de entidades, esto se logra moviendo un lugar las entidades que vengan después de aquella entidad que se quiere eliminar.
void entidad_eliminar(struct Entidad entidades[], int indice, int *contador)
{
    int count = *contador;
    for (int i = indice; i < count; i++)
        entidades[i] = entidades[i+1];
    *contador = count - 1;
}


// Su tarea es dibujar una entidad, basándose en sus coordenadasy tamaño
void entidad_dibujar(struct Entidad entidad)
{
    int flag = 0;

    al_draw_scaled_bitmap(entidad.sprite, 0, 0, al_get_bitmap_width(entidad.sprite), al_get_bitmap_height(entidad.sprite), entidad.x_pos, entidad.y_pos, entidad.ancho, entidad.alto, flag);
}


// Se encarga de mover una entidad dada en cierta dirección, varias llamadas conjuntas a esta funión pueden resultar en movimientos diagonales
// Toma como argumentos 2 apuntadores de las coordenadas de la entidad, tambien delta (su velocidad) y la direccion en la que se debe mover dicha entidad
int entidad_mover(struct Entidad *entidad, int comportamiento)
{
    int mov_valido = 1;
    entidad->y_pos += entidad->y_vel;
    entidad->x_pos += entidad->x_vel;
    if (comportamiento == BLOQUEO)
    {
        if (entidad->y_pos > ALTO-entidad->alto) 
            entidad->y_pos = ALTO-entidad->alto;
        else if (entidad->y_pos < 0)
            entidad->y_pos = 0;

        if (entidad->x_pos > ANCHO-entidad->ancho) 
            entidad->x_pos = ANCHO-entidad->ancho;
        else if (entidad->x_pos < 0)
            entidad->x_pos = 0;

    }
    else if (entidad->y_pos > ALTO || entidad->y_pos < 0 - entidad->alto || 
            entidad->x_pos > ANCHO|| entidad->x_pos < 0 - entidad->ancho) 
    {
        mov_valido = 0;
    }

    return mov_valido;
}

// La sunbrutina cambia los valores de velocidad en cada eje para que, en la siguiente llamada a "entidad_mover()", la entidad se mueva en el ángulo deseado (el ángulo debes estar en radianes). 
void cambiar_angulo_movimiento(struct Entidad *entidad, double angulo)
{
    entidad->x_vel = cos(angulo) * (entidad->max_vel);
    entidad->y_vel = sin(angulo) * (entidad->max_vel);
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


void modo_inicializar(struct Entidad entidades[], int modo, int *contador, struct Entidad *jugador)
{
    switch (modo)
    {
        case 0:
            al_play_sample(sonidos[MENU_TEMA], 0.20, 0, 1, ALLEGRO_PLAYMODE_LOOP, NULL); 
            entidad_crear(entidades, contador, FUEGO, NULL, NULL);
            entidad_crear(entidades, contador, FUEGO, NULL, NULL);
            entidades[0].x_pos = 15;
            entidades[0].y_pos = 100;

            entidades[1].x_pos = 480;
            entidades[1].y_pos = 105;
            break;
        case 1:
            al_play_sample(sonidos[JUEGO_TEMA], 0.20, 0, 1, ALLEGRO_PLAYMODE_LOOP, NULL); 
            entidad_crear(entidades, contador, GENERICO, NULL, imagenes[FONDO_0_IMAGEN]);
            entidades[0].x_pos = 0;
            entidades[0].y_pos = 0;
            entidades[0].x_vel = ((VEL/(float)FPS)/-8.0);
            entidad_crear(entidades, contador, GENERICO, NULL, imagenes[FONDO_0_IMAGEN]);
            entidades[1].x_pos = ANCHO * -1;
            entidades[1].y_pos = 0;
            entidades[1].x_vel = ((VEL/(float)FPS)/-8.0);

            entidad_crear(entidades, contador, GENERICO, NULL, imagenes[FONDO_2_IMAGEN]);
            entidades[2].x_pos = 0;
            entidades[2].y_pos = -50;
            entidades[2].x_vel = ((VEL/(float)FPS)/-5);
            entidad_crear(entidades, contador, GENERICO, NULL, imagenes[FONDO_2_IMAGEN]);
            entidades[3].x_pos = ANCHO * -1;
            entidades[3].y_pos = -50;
            entidades[3].x_vel = ((VEL/(float)FPS)/-5);

            entidad_crear(entidades, contador, GENERICO, NULL, imagenes[FONDO_3_IMAGEN]);
            entidades[4].x_pos = 0;
            entidades[4].y_pos = 50;
            entidades[4].x_vel = ((VEL/(float)FPS)/-4);
            entidad_crear(entidades, contador, GENERICO, NULL, imagenes[FONDO_3_IMAGEN]);
            entidades[5].x_pos = ANCHO * -1;
            entidades[5].y_pos = 50;
            entidades[5].x_vel = ((VEL/(float)FPS)/-4);

            entidad_inicializar(jugador, JUGADOR, NULL, NULL);
            break;
        case 2:
            al_play_sample(sonidos[CREDITOS_TEMA], 0.25, 0, 1, ALLEGRO_PLAYMODE_LOOP, NULL);
            break;
        default:
            break;
    }
}

// Esta subbrutina alterna entre los diferentes sprites que cada una de las entidades animadas tienen
void entidad_animar(struct Entidad *entidad)
{
    switch (entidad->tipo)
    {
        case JUGADOR:
            if (entidad->sprite == imagenes[JUGADOR_IMAGEN_0])
                entidad->sprite = imagenes[JUGADOR_IMAGEN_1];
            else
                entidad->sprite = imagenes[JUGADOR_IMAGEN_0];
            break;
        case MANTICORA:
            if (entidad->sprite == imagenes[MANTICORA_IMAGEN_0])
                entidad->sprite = imagenes[MANTICORA_IMAGEN_1];
            else
                entidad->sprite = imagenes[MANTICORA_IMAGEN_0];
            break;
        case FENIX:
            if (entidad->sprite == imagenes[FENIX_IMAGEN_0])
                entidad->sprite = imagenes[FENIX_IMAGEN_1];
            else
                entidad->sprite = imagenes[FENIX_IMAGEN_0];
            break;
        case GARGOLA:
            if (entidad->sprite == imagenes[GARGOLA_IMAGEN_0])
                entidad->sprite = imagenes[GARGOLA_IMAGEN_1];
            else
                entidad->sprite = imagenes[GARGOLA_IMAGEN_0];
            break;
        case HYDRA:
            if (entidad->sprite == imagenes[HYDRA_IMAGEN_0])
                entidad->sprite = imagenes[HYDRA_IMAGEN_1];
            else
                entidad->sprite = imagenes[HYDRA_IMAGEN_0];
            break;
        case FUEGO:
            if (entidad->sprite == imagenes[FUEGO_0_IMAGEN])
                entidad->sprite = imagenes[FUEGO_1_IMAGEN];
            else if (entidad->sprite == imagenes[FUEGO_1_IMAGEN])
                entidad->sprite = imagenes[FUEGO_2_IMAGEN];
            else
                entidad->sprite = imagenes[FUEGO_0_IMAGEN];
            break;
        default:
            break;
    }
}

int puntaje_mas_alto_obtener()
{
    int puntaje = 0;


    FILE *puntaje_archivo = fopen("puntaje.uwu", "rb");
    if (puntaje_archivo == NULL)
    {
        printf("Error al cargar el archivo");
    }
    else
    {
        fread(&puntaje, sizeof(int), 1, puntaje_archivo);
    }
    fclose(puntaje_archivo);

    return puntaje; 
}

void puntaje_mas_alto_guardar(int puntaje)
{
    
    FILE *puntaje_archivo = fopen("puntaje.uwu", "wb");
    if (puntaje_archivo == NULL)
    {
        printf("Error al cargar el archivo");
        return;
    }    
    rewind(puntaje_archivo);
    fwrite(&puntaje, sizeof(int), 1, puntaje_archivo);
    fclose(puntaje_archivo);
}
