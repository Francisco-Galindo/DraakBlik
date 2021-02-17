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

#include "util.c"


struct Entidad jugador;

int main()
{
    ALLEGRO_DISPLAY *disp;
    ALLEGRO_COLOR color_fondo;
    ALLEGRO_EVENT_QUEUE *eventos;
    ALLEGRO_EVENT evento;
    ALLEGRO_BITMAP *jugador_sprite;
    ALLEGRO_FONT *fuente60;
    ALLEGRO_FONT *fuente80;
    ALLEGRO_TIMER *timer = NULL;


    //ALLEGRO_BITMAP *candado_cerrado;

    //ALLEGRO_SAMPLE *sonido_exito;
    //ALLEGRO_SAMPLE *sonido_fracaso;

    int fin = 0, redibujar = 1, pausa = 0;

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

    jugador_sprite = al_load_bitmap("Imagenes/dragonopc.png");
    jugador.sprite = jugador_sprite;
    if (!jugador_sprite)
    {
        printf("No se cargaron las imagenes");
        fin =  1;
    }

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

    //sonido_exito = al_load_sample("musica/exito.mp3");
    //sonido_fracaso = al_load_sample("musica/error.mp3");

    fuente60 = al_load_font("Fuentes/PressStart2P.ttf", 40, 0);
    fuente80 = al_load_font("Fuentes/PressStart2P.ttf", 60, 0);

    if(!fuente60 || !fuente80)
    {
        printf("No se pudo cargar fuente");
        fin = 1;
    }

    disp = al_create_display(ANCHO,ALTO);
    al_set_window_title(disp, "Movimiento");
    al_set_display_icon(disp, jugador.sprite);


    color_fondo = al_map_rgb(20,20,20);
    al_clear_to_color(color_fondo);

    eventos = al_create_event_queue();
    timer = al_create_timer(1.0/FPS);
    al_register_event_source(eventos, al_get_display_event_source(disp));
    al_register_event_source(eventos, al_get_keyboard_event_source());
    al_register_event_source(eventos, al_get_timer_event_source(timer));

    inicializar_entidad(jugador, JUGADOR, NULL);
    al_start_timer(timer);
    al_flip_display();

    while (!fin)
    {
        if (redibujar == 1 && al_event_queue_is_empty(eventos))
        {
            al_clear_to_color(color_fondo);

            dibujar_entidad(jugador, jugador.x_pos, jugador.y_pos);
            if (pausa == 1)
                al_draw_text(fuente80, al_map_rgb(255, 255, 255), ANCHO/2, ALTO/2, ALLEGRO_ALIGN_CENTRE, "PAUSA");

            al_flip_display();

            redibujar = 0;

        }

        // Checamos si hay un evento
        al_wait_for_event(eventos, &evento);

        switch(evento.type)
        {
            case ALLEGRO_EVENT_TIMER:
                if(evento.timer.source == timer)
                {
                    if (pausa == 0)
                    {
                        if ((jugador.mov_aba || jugador.mov_arr) && (jugador.mov_izq || jugador.mov_der))
                        {
                            jugador.vel = VEL/1.41;
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
                    }
                    redibujar = 1;
                }
                break;
            case ALLEGRO_EVENT_KEY_CHAR:
                if(evento.keyboard.keycode == ALLEGRO_KEY_SPACE)
                {
                    pausa = !pausa;
                }
                break;
            case ALLEGRO_EVENT_KEY_DOWN:
                if(evento.keyboard.keycode == ALLEGRO_KEY_W || evento.keyboard.keycode == ALLEGRO_KEY_UP)
                {
                    jugador.mov_arr = 1;
                }
                else if (evento.keyboard.keycode == ALLEGRO_KEY_S || evento.keyboard.keycode == ALLEGRO_KEY_DOWN)
                {
                    jugador.mov_aba = 1;
                }
                if (evento.keyboard.keycode == ALLEGRO_KEY_A || evento.keyboard.keycode == ALLEGRO_KEY_LEFT) 
                {
                    jugador.mov_izq = 1;
                }
                else if (evento.keyboard.keycode == ALLEGRO_KEY_D || evento.keyboard.keycode == ALLEGRO_KEY_RIGHT) 
                {
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
