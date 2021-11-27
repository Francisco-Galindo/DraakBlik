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


enum modos {MENU, JUEGO, CREDITOS};

enum comportamientos {BLOQUEO, ELIMINAR};

enum tipos {JUGADOR, MANTICORA, FENIX, GARGOLA, HIDRA, FUEGO, JUGADOR_PROYECTIL, MANTICORA_PROYECTIL, FENIX_PROYECTIL, GARGOLA_PROYECTIL, HIDRA_PROYECTIL, GENERICO};

enum imagenes {JUGADOR_IMAGEN_0, JUGADOR_IMAGEN_1, CONTROL_0_IMAGEN, CONTROL_1_IMAGEN, ICONO_IMAGEN, WALL_IMAGEN, MONT1_IMAGEN, MONT2_IMAGEN, LUNA_IMAGEN, DRAGO_LUNA_IMAGEN, FUEGO_0_IMAGEN, FUEGO_1_IMAGEN, FUEGO_2_IMAGEN, FONDO_0_IMAGEN, FONDO_1_IMAGEN, FONDO_2_IMAGEN, FONDO_3_IMAGEN, TORRE_0_IMAGEN, TORRE_1_IMAGEN, EXPLOSION_IMAGEN, MANTICORA_IMAGEN_0, MANTICORA_IMAGEN_1, FENIX_IMAGEN_0, FENIX_IMAGEN_1, HIDRA_IMAGEN_0, HIDRA_IMAGEN_1, GARGOLA_IMAGEN_0, GARGOLA_IMAGEN_1, PROYECTIL_0_IMAGEN, PROYECTIL_1_IMAGEN, PROYECTIL_2_IMAGEN, PROYECTIL_3_IMAGEN, PROYECTIL_4_IMAGEN};

enum fuentes {FUENTE_60, FUENTE_40, FUENTE_20, FUENTE_15,FUENTE_TITULO_20, FUENTE_TITULO_80, FUENTE_TITULO_50};

enum sonidos {MENU_TEMA, CREDITOS_TEMA, JUEGO_TEMA, SELECCION_SONIDO, DANO_SONIDO, DISPARO_SONIDO, VIDA_SONIDO, EXPLOSION_SONIDO};

// Declaración de la estructura de una entidad, en la que se basan todos los personajes y proyectiles del juego
struct Entidad
{
    int tipo;  /* El tipo de entidad */
    ALLEGRO_BITMAP *sprite; /* La imagen con la que se dibujará */
    float x_pos; /* Posición en el eje X y Y */
    float y_pos;
    float max_vel; /* Velocidad máxima a la que se puede mover la entidad */
    float x_vel; /* Velocidad en cada uno de los ejes */
    float y_vel;
    int vidas;  /* Indica con cuantas vidas cuenta la entidad */
    float alto; /* "alto" y "ancho" indican el tamaño de la entidad en ambas dimensiones */
    float ancho;
};




ALLEGRO_BITMAP *imagenes[33];
ALLEGRO_FONT *fuentes[7];
ALLEGRO_SAMPLE *sonidos[8];


// Aquí cargamos todas las imágenes que serán usadas en el juego, cada una se asigna a una localidad del arreglo de apuntadores a imágenes.
void imagenes_cargar(int *fin)
{
    imagenes[JUGADOR_IMAGEN_0] = al_load_bitmap("Imagenes/drav.png");
    imagenes[JUGADOR_IMAGEN_1] = al_load_bitmap("Imagenes/drav_abajo.png");
    imagenes[CONTROL_0_IMAGEN] = al_load_bitmap("Imagenes/Flechjuego.png");
    imagenes[CONTROL_1_IMAGEN] = al_load_bitmap("Imagenes/Wasdjuego.png");
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
    imagenes[TORRE_0_IMAGEN] = al_load_bitmap("Imagenes/Torre3.png");
    imagenes[TORRE_1_IMAGEN] = al_load_bitmap("Imagenes/Torre4.png");
    imagenes[EXPLOSION_IMAGEN] = al_load_bitmap("Imagenes/Explosion.png");
    imagenes[MANTICORA_IMAGEN_0] = al_load_bitmap("Imagenes/quimera.png");
    imagenes[MANTICORA_IMAGEN_1] = al_load_bitmap("Imagenes/quimera_abajo.png");
    imagenes[FENIX_IMAGEN_0] = al_load_bitmap("Imagenes/fenix_izquierda.png");
    imagenes[FENIX_IMAGEN_1] = al_load_bitmap("Imagenes/fenix.png");
    imagenes[HIDRA_IMAGEN_0] = al_load_bitmap("Imagenes/hidra.png");
    imagenes[HIDRA_IMAGEN_1] = al_load_bitmap("Imagenes/hidra_abajo.png");
    imagenes[GARGOLA_IMAGEN_0] = al_load_bitmap("Imagenes/gargola_arriba.png");
    imagenes[GARGOLA_IMAGEN_1] = al_load_bitmap("Imagenes/gargola_abajo.png");
    imagenes[PROYECTIL_0_IMAGEN] = al_load_bitmap("Imagenes/disparo_dragon.png");
    imagenes[PROYECTIL_1_IMAGEN] = al_load_bitmap("Imagenes/disparo_fenix.png");
    imagenes[PROYECTIL_2_IMAGEN] = al_load_bitmap("Imagenes/disparo_hidra.png");
    imagenes[PROYECTIL_3_IMAGEN] = al_load_bitmap("Imagenes/disparo_gargola.png");
    imagenes[PROYECTIL_4_IMAGEN] = al_load_bitmap("Imagenes/disparo_manticora.png");

    // Se itera sobre todas las imagenes para checar si se han cargado correctamente, de lo contrario, avisarlo.
    for (int i = 0; i <= PROYECTIL_4_IMAGEN; i++)
    {
        if (!imagenes[i])
        {
            printf("No se ha podido cargar la imagen %i\n", i);
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
    for (int i = 0; i <= FUENTE_TITULO_50; i++)
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
    sonidos[MENU_TEMA] = al_load_sample("Audio/soundtrack_menu.wav");
    sonidos[CREDITOS_TEMA] = al_load_sample("Audio/soundtrack_creditos.wav");
    sonidos[JUEGO_TEMA] = al_load_sample("Audio/soundtrack_juego.wav");
    sonidos[SELECCION_SONIDO] = al_load_sample("Audio/select.wav");
    sonidos[DANO_SONIDO] = al_load_sample("Audio/dano.wav");
    sonidos[DISPARO_SONIDO] = al_load_sample("Audio/disparo_sonido.wav");
    sonidos[VIDA_SONIDO] = al_load_sample("Audio/vida_sonido.wav");
    sonidos[EXPLOSION_SONIDO] = al_load_sample("Audio/explosion_sonido.wav");
    for (int i = 0; i <= EXPLOSION_SONIDO; i++)
    {
        if(!sonidos[i])
        {
            printf("No se ha podido cargar el sonido %i\n", i);
            *fin =  1;
        }
    }
}


// Se encarga de inicializar una entidad, el primer argumento es la entidad que se quiere inicializar; el segundo representa qué tipo de entidad es; el tercero es un apuntador a otra entidad de referencia, en caso de ser necesario.
void entidad_inicializar(struct Entidad *entidad, int tipo, struct Entidad *entidad_origen, ALLEGRO_BITMAP *imagen)
{
    // Asignando los valores de algunos campos comúnes a la entidad.
    entidad->x_pos = 0;
    entidad->y_pos = 0;
    entidad->x_vel = 0;
    entidad->y_vel = 0;
    entidad->vidas = 1;
    entidad->tipo = tipo;

    // Se le asignan otros campos dependiendo del tipo de entidad que sea.
    switch (tipo)
    {
        case JUGADOR:
            entidad->sprite = imagenes[JUGADOR_IMAGEN_0];
            entidad->y_pos = ALTO/2 - (entidad->alto / 2);
            entidad->max_vel = VEL/FPS;
            entidad->vidas = 5;
            entidad->alto = al_get_bitmap_height(entidad->sprite)*1;
            entidad->ancho = al_get_bitmap_width(entidad->sprite)*1;
            break;
        case JUGADOR_PROYECTIL:
            entidad->sprite = imagenes[PROYECTIL_0_IMAGEN];
            entidad->max_vel = (VEL/FPS) * 2;
            entidad->x_vel = entidad->max_vel;
            entidad->alto = al_get_bitmap_height(entidad->sprite)*0.125;
            entidad->ancho = al_get_bitmap_width(entidad->sprite)*0.125;
            entidad->x_pos = entidad_origen->x_pos + (entidad_origen->ancho / 2);
            entidad->y_pos = entidad_origen->y_pos + (entidad_origen->alto / 2);
            break;
        case MANTICORA:
            entidad->sprite = imagenes[MANTICORA_IMAGEN_0];
            entidad->x_pos = ANCHO-100;
            entidad->y_pos = rand()%ALTO;
            entidad->max_vel = (VEL/FPS)/2;
            entidad->alto = al_get_bitmap_height(entidad->sprite)*1.25;
            entidad->ancho = al_get_bitmap_width(entidad->sprite)*1.25;
            entidad->x_vel = entidad->max_vel/2;
            entidad->y_vel = entidad->max_vel/2;
            break;
        case MANTICORA_PROYECTIL:
            entidad->sprite = imagenes[PROYECTIL_4_IMAGEN];
            entidad->x_pos = entidad_origen->x_pos + (entidad_origen->ancho / 2);
            entidad->y_pos = entidad_origen->y_pos + (entidad_origen->alto / 2);
            entidad->max_vel = ((VEL/FPS)*0.8);
            entidad->x_vel = entidad->max_vel;
            entidad->alto = al_get_bitmap_height(entidad->sprite)*0.125;
            entidad->ancho = al_get_bitmap_width(entidad->sprite)*0.125;
            break;
        case FENIX:
            entidad->sprite = imagenes[FENIX_IMAGEN_0];
            entidad->x_pos = ANCHO-100;
            entidad->y_pos = rand()%ALTO;
            entidad->max_vel = (VEL/FPS)/2;
            entidad->alto = al_get_bitmap_height(entidad->sprite)*0.1;
            entidad->ancho = al_get_bitmap_width(entidad->sprite)*0.1;
            break;
        case FENIX_PROYECTIL:
            entidad->sprite = imagenes[PROYECTIL_1_IMAGEN];
            entidad->x_pos = entidad_origen->x_pos + (entidad_origen->ancho / 2);
            entidad->y_pos = entidad_origen->y_pos + (entidad_origen->alto / 2);
            entidad->max_vel = (VEL/FPS)*0.8;
            entidad->x_vel = entidad->max_vel;
            entidad->alto = al_get_bitmap_height(entidad->sprite)*0.125;
            entidad->ancho = al_get_bitmap_width(entidad->sprite)*0.125;
            break;
        case GARGOLA:
            entidad->sprite = imagenes[GARGOLA_IMAGEN_0];
            entidad->x_pos = ANCHO-100;
            entidad->y_pos = rand()%ALTO;
            entidad->max_vel = (VEL/FPS)/3;
            entidad->alto = al_get_bitmap_height(entidad->sprite)*1;
            entidad->ancho = al_get_bitmap_width(entidad->sprite)*1;
            break;
        case GARGOLA_PROYECTIL:
            entidad->sprite = imagenes[PROYECTIL_3_IMAGEN];
            entidad->max_vel = (VEL/FPS)*0.8;
            entidad->x_vel = entidad->max_vel * -1;
            entidad->alto = al_get_bitmap_height(entidad->sprite)*0.125;
            entidad->ancho = al_get_bitmap_width(entidad->sprite)*0.125;
            entidad->x_pos = entidad_origen->x_pos + (entidad_origen->ancho / 2);
            entidad->y_pos = entidad_origen->y_pos + (entidad_origen->alto / 2);
            break;
        case HIDRA:
            entidad->sprite = imagenes[HIDRA_IMAGEN_0];
            entidad->x_pos = ANCHO-160;
            entidad->max_vel = (VEL/FPS)/3;
            entidad->alto = al_get_bitmap_height(entidad->sprite)*1;
            entidad->ancho = al_get_bitmap_width(entidad->sprite)*1;
            entidad->y_pos = 0;
            break;
        case HIDRA_PROYECTIL:
            entidad->sprite = imagenes[PROYECTIL_2_IMAGEN];
            entidad->max_vel = VEL/FPS;
            entidad->x_vel = entidad->max_vel;
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


// La subrutina toma como parámetro el arreglo de todas las entidades, junto con el tipo de entidad que se quiere crear, y una entidad de origen en caso de ser necesaria. Se encarga de añadir dicha entidad al arreglo y aumentar el contador de entidades.
void entidad_crear(struct Entidad entidades[], int *contador, int tipo, struct Entidad *entidad_origen, ALLEGRO_BITMAP *imagen)
{
    int count = *contador;
    entidad_inicializar(&entidades[count], tipo, entidad_origen, imagen);
    *contador = count + 1;
}


// Esta subrutina se usa para poder "eliminar" una entidad del arreglo de entidades, esto se logra moviendo un lugar las entidades que vengan después de aquella entidad que se quiere eliminar.
void entidad_destruir(struct Entidad entidades[], int indice, int *contador)
{
    int count = *contador;
    for (int i = indice; i < count; i++)
        entidades[i] = entidades[i+1];
    *contador = count - 1;
}

// Hace lo que su nombre indica, toma de parámetros el arreglo del que se va a destruir la entidad, el índice en el que se encuentra, un apuntador al contador de entidades, un apuntador al puntaje del jugador, y un apuntador al jugador.
int entidad_destruir_si_esta_muerta(struct Entidad entidades[], int indice, int *contador_de_entidades, int *puntaje, struct Entidad *jugador)
{
    int tipo = entidades[indice].tipo;
    if (entidades[indice].vidas <= 0)
    {
        if (puntaje != NULL && jugador != NULL)
        {
            int puntaje_nuevo = *puntaje;
            if (jugador->vidas > 0)
            {
                switch (tipo)
                {
                    case GARGOLA:
                        puntaje_nuevo += 3;
                        break;
                    case MANTICORA:
                        puntaje_nuevo += 5;
                        break;
                    case HIDRA:
                        puntaje_nuevo += 10;
                        break;
                    case FENIX:
                        puntaje_nuevo += 15;
                        break;
                    default:
                        break;
                }
            }
            *puntaje = puntaje_nuevo;
        }
        al_play_sample(sonidos[EXPLOSION_SONIDO], 0.1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
        entidad_destruir(entidades, indice, contador_de_entidades);
        return tipo;
    }
    return 0;
}

// Su tarea es dibujar una entidad, basándose en sus coordenadasy tamaño.
void entidad_dibujar(struct Entidad entidad)
{
    int flag = 0;

    al_draw_scaled_bitmap(entidad.sprite, 0, 0, al_get_bitmap_width(entidad.sprite), al_get_bitmap_height(entidad.sprite), entidad.x_pos, entidad.y_pos, entidad.ancho, entidad.alto, flag);
}


/*
 Se encarga de mover una entidad dada en cierta dirección, varias llamadas conjuntas a esta funión pueden resultar en movimientos diagonales
Toma como argumentos 2 apuntadores de las coordenadas de la entidad, tambien delta (su velocidad) y la direccion en la que se debe mover dicha entidad.
*/
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


void entidad_perseguir(struct Entidad *entidad_a_mover, struct Entidad entidad_destino)
{
    // Vamos a imaginar la velocidad en cada eje de una entidad como los componentes de un vector, así, sabiendo la posición de una entidad de destino, podemos hacer que el vector de movimiento apunte hacia allá

    // Calculando la distancia entre el proyectil y el jugador
    /*
    Ya existe una función designada a medir la distancia entre dos entidades, pero para sernos útil aquí, también necesitamos saber la distancia en cada eje.
    El equipo sintió que modificar la función para esto sería complicarla de más, por la manera en que tendríamos que regresar los valores de salida).
    */

    float dis_x = (entidad_a_mover->x_pos +
                   (entidad_a_mover->ancho / 2)) -
                    (entidad_destino.x_pos + (entidad_destino.ancho / 2));
    float dis_y = (entidad_a_mover->y_pos +
                   (entidad_a_mover->alto / 2)) -
                   (entidad_destino.y_pos + (entidad_destino.alto / 2));
    double dis_total = pow((dis_x * dis_x) + (dis_y * dis_y), 0.5);

    // Modificando la magnitud del vector para que sea igual a la velocidad máxima de movimiento de la entidad a mover
    double const_proporcionalidad = dis_total/entidad_a_mover->max_vel;
    entidad_a_mover->x_vel = dis_x/const_proporcionalidad * -1;
    entidad_a_mover->y_vel = dis_y/const_proporcionalidad * -1;
}

// Esta subrutina alterna entre los diferentes sprites que cada una de las entidades animadas tienen.
void entidad_animar(struct Entidad *entidad)
{
    // En cada uno de los casos del switch, la imagen de la entidad en cuestión será cambiada por la siguiente para animarla.
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
        case HIDRA:
            if (entidad->sprite == imagenes[HIDRA_IMAGEN_0])
                entidad->sprite = imagenes[HIDRA_IMAGEN_1];
            else
                entidad->sprite = imagenes[HIDRA_IMAGEN_0];
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

// Función que checa si una entidad colisiona con otra, se usa el método "Axis-Aligned Bounding Box" (AABB).
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

// Función que usa el teorema de Pitágoras para calcular la distancia entre el centro de dos entidades.
double distancia_entre_entidades(struct Entidad entidad_uno, struct Entidad entidad_dos)
{
    // Calculando la distancia entre el proyectil y el jugador
    float dis_x = (entidad_uno.x_pos +
                   (entidad_uno.ancho / 2)) -
                    (entidad_dos.x_pos + (entidad_dos.ancho / 2));
    float dis_y = (entidad_uno.y_pos +
                   (entidad_uno.alto / 2)) -
                   (entidad_dos.y_pos + (entidad_dos.alto / 2));
    double dis_total = pow((dis_x * dis_x) + (dis_y * dis_y), 0.5);

    return dis_total;
}

// Se encarga de reducir la vida de dos entidades en caso de que colisionen, regresa un entero representando si se encuentran colisiones.
int entidades_reducir_vida_si_colisionan(struct Entidad *entidad_uno, struct Entidad *entidad_dos, int *marcar_como_danado)
{
    int colisionaron = 0;

    if (colisiona_AABB(*entidad_uno, *entidad_dos))
    {
        if (marcar_como_danado != NULL)
        {
            al_play_sample(sonidos[DANO_SONIDO], 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
            *marcar_como_danado = 1;
        }

        entidad_uno->vidas --;
        entidad_dos->vidas--;

        colisionaron = 1;
    }
    return colisionaron;
}


// Aquí se inicializa cada uno de los modos del juego.
void modo_inicializar(struct Entidad entidades[], int *contador, int modo,  struct Entidad *jugador)
{
    switch (modo)
    {
        case MENU:
            // Se reproduce el tema del menú.
            al_play_sample(sonidos[MENU_TEMA], 0.20, 0, 1, ALLEGRO_PLAYMODE_LOOP, NULL);
            // Se crean dos entidades tipo fuego para poder dibujarlas en el menú.
            entidad_crear(entidades, contador, FUEGO, NULL, NULL);
            entidad_crear(entidades, contador, FUEGO, NULL, NULL);
            entidades[0].x_pos = 15;
            entidades[0].y_pos = 100;

            entidades[1].x_pos = 480;
            entidades[1].y_pos = 105;
            break;
        case JUEGO:

            // Se reproduce el tema del juego.
            al_play_sample(sonidos[JUEGO_TEMA], 0.20, 0, 1, ALLEGRO_PLAYMODE_LOOP, NULL);

            /*
            Se crean entidades para representar las montañas de fondo, se hace en pares, donde se mueva una entidad detrás de otra para poder crear la ilusión de que es una sóla imágen con longitud infinita
            */
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
        case CREDITOS:

            // Sólamente es necesario reproducir el tema de los créditos
            al_play_sample(sonidos[CREDITOS_TEMA], 0.25, 0, 1, ALLEGRO_PLAYMODE_LOOP, NULL);
            break;
        default:
            break;
    }
}



// Abre el archivo donde se guardan los puntajes, lee y regresa el puntaje guarado ahí.
int puntaje_mas_alto_obtener()
{
    int puntaje = 0;


    FILE *puntaje_archivo = fopen("puntaje.uwu", "rb");
    if (puntaje_archivo == NULL)
    {
        printf("Error al cargar el archiv\n");
    }
    else
    {
        fread(&puntaje, sizeof(int), 1, puntaje_archivo);
    }
    fclose(puntaje_archivo);

    return puntaje;
}

// Abre el archivo donde se guardan los puntajes, y escribe ahí el puntaje que recibió como parámetro de entrada.
void puntaje_mas_alto_guardar(int puntaje)
{

    FILE *puntaje_archivo = fopen("puntaje.uwu", "wb");
    if (puntaje_archivo == NULL)
    {
        printf("Error al cargar el archivo\n");
        return;
    }
    rewind(puntaje_archivo);
    fwrite(&puntaje, sizeof(int), 1, puntaje_archivo);
    fclose(puntaje_archivo);
}
