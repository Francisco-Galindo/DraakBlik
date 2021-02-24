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

#include "menuYCreditos.c"

struct Entidad jugador;
struct Entidad entidades[128];
struct Entidad entidades_no_vivas[64];

int main()
{
    srand(time(NULL));
    ALLEGRO_DISPLAY *disp;
    ALLEGRO_COLOR color_fondo;
    ALLEGRO_EVENT_QUEUE *eventos;
    ALLEGRO_EVENT evento;
    ALLEGRO_TIMER *framerate = NULL;
    ALLEGRO_TIMER *anim = NULL;


    //ALLEGRO_SAMPLE *sonido_exito;
    //ALLEGRO_SAMPLE *sonido_fracaso;

    int fin = 0, redibujar = 1, pausa = 0, mode = 0, num_entidades = 0, inertes = 0, proyectiles_jugador = 0, ops = 0;

    // Cargando los elementos de Allegro
    if(!al_init())
    {
        printf("Todo murio :(");
        fin = 1;
    }

    if(!al_init_font_addon() || !al_init_ttf_addon())
    {
        printf("No se pudieronn cargar las fuentes lol");
        fin = 1;
    }
    
    if(!al_install_keyboard())
    {
      printf("No carga el coomplemento de teclados");
      fin = 1;
    }

    if(!al_init_image_addon())
    {
        printf("Imagenes error");
        fin = 1;
    }

    cargar_imagenes(&fin);

    if (!al_install_audio())
    {
        printf("No se pudo cargar complemmento de audio");
        fin = 1;
    }

    if (!al_init_acodec_addon())
    {
        printf("No se pudo cargar el codec");
        fin = 1;
    }

    if (!al_reserve_samples(2))
    {
        printf("No se pudo cargar mixer");
        fin = 1;
    }

    cargar_fuentes(&fin);

    //sonido_exito = al_load_sample("musica/exito.mp3");
    //sonido_fracaso = al_load_sample("musica/error.mp3");

    // Creando la ventana
    disp = al_create_display(ANCHO,ALTO);
    al_set_window_title(disp, "Movimiento");
    al_set_display_icon(disp, imagenes[ICONO_IMAGEN]);

    color_fondo = al_map_rgb(1, 1, 31);
    al_clear_to_color(color_fondo);

    // Creando los eventos que van a ser usados en el juego
    eventos = al_create_event_queue();
    framerate = al_create_timer(1.0/FPS);
    anim = al_create_timer(0.15);
    al_register_event_source(eventos, al_get_display_event_source(disp));
    al_register_event_source(eventos, al_get_keyboard_event_source());
    al_register_event_source(eventos, al_get_timer_event_source(framerate));
    al_register_event_source(eventos, al_get_timer_event_source(anim));
    
    inicializar_entidad(&jugador, JUGADOR, NULL, NULL);
    inicializar_modo(entidades_no_vivas, mode, &inertes, NULL);
    al_start_timer(framerate);
    al_start_timer(anim);
    al_flip_display();


    while (!fin)
    {
        // Dibujando pantalla
        if (redibujar == 1 && al_event_queue_is_empty(eventos))
        {
            al_clear_to_color(color_fondo);
            if (mode == 0)
            {
                dibujar_menu(entidades_no_vivas, &ops);
            }
            else if (mode == 1)
            { 
                for (int i = 0; i < num_entidades; i++)
                    dibujar_entidad(entidades[i]);

                dibujar_entidad(jugador);

                if (pausa == 1)
                    al_draw_text(fuentes[FUENTE_60], al_map_rgb(255, 255, 255), ANCHO/2, ALTO/2, ALLEGRO_ALIGN_CENTRE, "PAUSA");

            }
            else if (mode == 2)
            {
                dibujar_creditos();
            }

            al_flip_display();
            redibujar = 0;

        }

        // Checamos si hay un evento
        al_wait_for_event(eventos, &evento);

        switch(evento.type)
        {
            case ALLEGRO_EVENT_TIMER:
                // Cada que un nuevo cuadro deba ser dibujado (indicado por el timer "framerate"), realizar cálculos para actualizar el estado del juego, para luego cambiar el valor de la variable de dibujar a 1.
                if(evento.timer.source == framerate)
                {
                    if (pausa == 0 && mode == 1)
                    {
                        // Spawneo de las entidades
                        if (num_entidades < 10)
                        {
                            if (rand()%50 == 3)
                            {
                                crear_entidad(entidades, &num_entidades, MANTICORA, NULL, NULL); 
                                printf("%i\n", num_entidades);
                            }
                        }

                        // Movimiento del jugador
                        if ((jugador.mov_aba || jugador.mov_arr) && (jugador.mov_izq || jugador.mov_der))
                        {
                            jugador.vel = (int)VEL/RAIZ_DOS;
                        }
                        else
                            jugador.vel = VEL; 

                        if (jugador.mov_arr)
                            mover_entidad(&jugador.x_pos, &jugador.y_pos, jugador.vel, ARRIBA, BLOQUEO);
                        if (jugador.mov_aba)
                            mover_entidad(&jugador.x_pos, &jugador.y_pos, jugador.vel, ABAJO, BLOQUEO);
                        if (jugador.mov_izq)
                            mover_entidad(&jugador.x_pos, &jugador.y_pos, jugador.vel, IZQUIERDA, BLOQUEO);
                        if (jugador.mov_der)
                            mover_entidad(&jugador.x_pos, &jugador.y_pos, jugador.vel, DERECHA, BLOQUEO);

                        // Checando colisiones y actualizando las vidas de las entidades correspondientes
                        for (int i = 0; i < num_entidades; i++)
                        {
                            if (colisiona_AABB(jugador, entidades[i]))
                            {
                                jugador.vidas --;
                                entidades[i].vidas --;
                                if (jugador.vidas <= 0);
                                    //asdfasdf
                                if (entidades[i].vidas <= 0)
                                    eliminar_entidad(entidades, i, &num_entidades);
                            }

                        }
                    }
                    redibujar = 1;
                }
                else if (evento.timer.source == anim)
                {
                    if (mode == 0)
                    {
                        dibujar_fuego(&entidades_no_vivas[0], imagenes[FUEGO_0_IMAGEN], imagenes[FUEGO_1_IMAGEN], imagenes[FUEGO_2_IMAGEN]);
                        dibujar_fuego(&entidades_no_vivas[1], imagenes[FUEGO_0_IMAGEN], imagenes[FUEGO_1_IMAGEN], imagenes[FUEGO_2_IMAGEN]);

                    }
                }
                break;
            
            
            case ALLEGRO_EVENT_KEY_DOWN:
                if(evento.keyboard.keycode == ALLEGRO_KEY_W || evento.keyboard.keycode == ALLEGRO_KEY_UP)
                {
                    if (mode == 1)
                        jugador.mov_arr = 1;
                                        else
                    if (ops>0)
                        ops--;
                }
                else if (evento.keyboard.keycode == ALLEGRO_KEY_S || evento.keyboard.keycode == ALLEGRO_KEY_DOWN)
                {
                    if (mode == 1)
                        jugador.mov_aba = 1;
                    else
                        if (ops<2)
                            ops++;
                }
                if (evento.keyboard.keycode == ALLEGRO_KEY_A || evento.keyboard.keycode == ALLEGRO_KEY_LEFT) 
                {
                    if (mode == 1)
                        jugador.mov_izq = 1;
                }
                else if (evento.keyboard.keycode == ALLEGRO_KEY_D || evento.keyboard.keycode == ALLEGRO_KEY_RIGHT) 
                {
                    if (mode == 1)
                        jugador.mov_der = 1;
                }
                break;
            
            case ALLEGRO_EVENT_KEY_UP:
                
                if(evento.keyboard.keycode == ALLEGRO_KEY_W || evento.keyboard.keycode == ALLEGRO_KEY_UP)
                {
                    jugador.mov_arr = 0;
                }
                else if (evento.keyboard.keycode == ALLEGRO_KEY_S || evento.keyboard.keycode == ALLEGRO_KEY_DOWN)
                {
                    jugador.mov_aba = 0;
                }
                if (evento.keyboard.keycode == ALLEGRO_KEY_A || evento.keyboard.keycode == ALLEGRO_KEY_LEFT)
                {
                    jugador.mov_izq = 0;
                }
                else if (evento.keyboard.keycode == ALLEGRO_KEY_D || evento.keyboard.keycode == ALLEGRO_KEY_RIGHT) 
                {
                    jugador.mov_der = 0;
                }
                break;
            
            case ALLEGRO_EVENT_KEY_CHAR:
                if(evento.keyboard.keycode == ALLEGRO_KEY_SPACE)
                {
                    if (mode == 1)
                        pausa = !pausa;
                }
                else if (evento.keyboard.keycode == ALLEGRO_KEY_ENTER)
                {
                    if (ops == 0 && mode == 0)
                    {
                        inertes = 0;
                        num_entidades = 0;
                        mode = 1;
                        inicializar_modo(entidades, mode, &num_entidades, &jugador);
                    }
                    else if (ops == 1)
                    {
                        inertes = 0;
                        num_entidades = 0;
                        mode = 2;
                    }
                    else if (ops == 2)
                        fin = 1;
                }
                break;
            
            case ALLEGRO_EVENT_DISPLAY_SWITCH_OUT:
                pausa = 1;
                printf("\nMinimizado\n");
                break;
            case ALLEGRO_EVENT_DISPLAY_RESIZE:
                printf("\nResize\n");
                break;
            case ALLEGRO_EVENT_DISPLAY_SWITCH_IN:
                printf("\nMaxim\n");
                redibujar = 1;
                break;
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                printf("\nBye\n");
                fin = 1;
                break;
                // Comentario
        }


    }

    al_destroy_event_queue(eventos);
    al_destroy_display(disp);
    return 0;
}
