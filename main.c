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

#include "menuYCreditos.c"

struct Entidad jugador;
struct Entidad entidades[128];
struct Entidad proyectiles_jugador[128];
struct Entidad proyectiles_enemigo[128];
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


    int fin = 0, redibujar = 1, pausa = 1, mode = 0, num_entidades = 0, num_inertes = 0, num_proyectiles_jugador = 0, num_proyectiles_enemigos = 0, ops = 0, danado = 0;

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
    
    fuentes_cargar(&fin);

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

    imagenes_cargar(&fin);

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

    if (!al_reserve_samples(32))
    {
        printf("No se pudo cargar mixer");
        fin = 1;
    }
    sonidos_cargar(&fin);


    // Creando la ventana
    disp = al_create_display(ANCHO,ALTO);
    al_set_window_title(disp, "Draak Blik");
    al_set_display_icon(disp, imagenes[ICONO_IMAGEN]);

    color_fondo = al_map_rgb(1, 1, 31);
    al_clear_to_color(color_fondo);

    // Creando los eventos que van a ser usados en el juego
    eventos = al_create_event_queue();
    framerate = al_create_timer(1.0/FPS);
    anim = al_create_timer(0.075);
    al_register_event_source(eventos, al_get_display_event_source(disp));
    al_register_event_source(eventos, al_get_keyboard_event_source());
    al_register_event_source(eventos, al_get_timer_event_source(framerate));
    al_register_event_source(eventos, al_get_timer_event_source(anim));

    entidad_inicializar(&jugador, JUGADOR, NULL, NULL);
    modo_inicializar(entidades_no_vivas, mode, &num_inertes, NULL);
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
                for (int i = 0; i < num_inertes; i++)
                    entidad_dibujar(entidades_no_vivas[i]); 

                for (int i = 0; i < num_entidades; i++)
                    entidad_dibujar(entidades[i]);

                for (int i = 0; i < num_proyectiles_jugador; i++)                
                    entidad_dibujar(proyectiles_jugador[i]);

                for (int i = 0; i < num_proyectiles_enemigos; i++)
                {
                    entidad_dibujar(proyectiles_enemigo[i]);
                }
                

                if ((danado == 0 || pausa == 1) || al_get_timer_count(anim) % 3 == 0)
                    entidad_dibujar(jugador);

                if (pausa == 1)
                {
                    al_draw_text(fuentes[FUENTE_60], al_map_rgb(255, 255, 255), ANCHO/2, ALTO/2.5, ALLEGRO_ALIGN_CENTRE, "PAUSA");
                    al_draw_text(fuentes[FUENTE_15], al_map_rgb(255, 255, 255), ANCHO/2, ALTO/1.5, ALLEGRO_ALIGN_CENTRE, "PRESIONA ENTER PARA IR AL MENU");
                }

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
                // Cada que un nuevo cuadro deba ser dibujado (indicado por el
                // timer "framerate"), realizar cálculos para actualizar el 
                // estado del juego, para luego cambiar el valor de la variable de dibujar a 1.
                if(evento.timer.source == framerate)
                {
                    if (pausa == 0 && mode == 1)
                    {
                        if(danado == 1 && al_get_timer_count(framerate) == FPS/0.75)
                        {
                            danado = 0;
                        }
                        // Spawneo de las entidades
                        if (num_entidades < 5)
                        {
                            if (rand()%FPS == 3)
                            {
                                int tipo = rand()%4+2;
                                entidad_crear(entidades, &num_entidades, tipo, NULL, NULL); 
                                printf("%i\n", num_entidades);
                            }
                        }

                        // Movimiento del jugador
                        entidad_mover(&jugador, BLOQUEO);

                        // Actualización en el estado de los enemigos
                        for (int i = 0; i < num_entidades; i++)
                        {
                            // Hacer que las manticoras disparen en dirección del enemigo de manera aleatoria
                            if (entidades[i].tipo == MANTICORA && rand()%(FPS * 2) == 2)
                            {
                                entidad_crear(proyectiles_enemigo, &num_proyectiles_enemigos, PROYECTIL_MANTICORA, &entidades[i], NULL);

                                // Variable que indica el indice del proyectil del que queremos calcular el vector de movimiento
                                int indice = num_proyectiles_enemigos-1;

                                // Vamos a imaginar la velocidad en cada eje del proyectil como los componentes de un vector, así, sabiendo la posición del jugador, podemos hacer que el proyectil vaya hacia él

                                // Calculando la distancia entre el proyectil y el jugador
                                float dis_x = (proyectiles_enemigo[indice].x_pos + 
                                            (proyectiles_enemigo[indice].ancho / 2)) - 
                                            (jugador.x_pos + (jugador.ancho / 2));
                                float dis_y = (proyectiles_enemigo[indice].y_pos + 
                                            (proyectiles_enemigo[indice].alto / 2)) - 
                                            (jugador.y_pos + (jugador.alto / 2));
                                double dis_total = pow((dis_x * dis_x) + (dis_y * dis_y), 0.5);

                                // Modificando la magnitud del vector para que sea igual a la velocidad máxima de movimiento de este proyectil
                                double const_proporcionalidad = dis_total/proyectiles_enemigo[indice].max_vel;
                                proyectiles_enemigo[indice].x_vel = dis_x/const_proporcionalidad;
                                proyectiles_enemigo[indice].y_vel = dis_y/const_proporcionalidad;
                            }
                            // Las gárgolas dispararán aleatoriamente también
                            else if (entidades[i].tipo == GARGOLA && rand()%FPS*1.5 == 2)
                            {
                                entidad_crear(proyectiles_enemigo, &num_proyectiles_enemigos, PROYECTIL_GARGOLA, &entidades[i], NULL);
                            }
                            else if (entidades[i].tipo == HYDRA && rand()%FPS*2 == 2)
                            {                                
                                // Creando tres proyectiles que se muevan en una manera similar a la de una escopeta
                                entidad_crear(proyectiles_enemigo, &num_proyectiles_enemigos, PROYECTIL_HYDRA, &entidades[i], NULL);
                                cambiar_angulo_movimiento(&proyectiles_enemigo[num_proyectiles_enemigos-1], PI/6.0);

                                entidad_crear(proyectiles_enemigo, &num_proyectiles_enemigos, PROYECTIL_HYDRA, &entidades[i], NULL);
                                cambiar_angulo_movimiento(&proyectiles_enemigo[num_proyectiles_enemigos-1], 0);

                                entidad_crear(proyectiles_enemigo, &num_proyectiles_enemigos, PROYECTIL_HYDRA, &entidades[i], NULL);
                                cambiar_angulo_movimiento(&proyectiles_enemigo[num_proyectiles_enemigos-1], PI/-6.0);

                            }


                            if (danado == 0 && colisiona_AABB(jugador, entidades[i]))
                            {
                                al_play_sample(sonidos[DANO_SONIDO], 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
                                danado = 1;
                                al_set_timer_count(framerate, 0);
                                jugador.vidas --;
                                entidades[i].vidas --;
                                if (jugador.vidas <= 0);
                                    //printf("lol");
                                if (entidades[i].vidas <= 0)
                                    entidad_eliminar(entidades, i, &num_entidades);
                            }

                        }

                        // Actualización de estado de cada proyectil del jugador
                        for (int i = 0; i < num_proyectiles_jugador; i++)
                        {
                            if (!entidad_mover(&proyectiles_jugador[i], ELIMINAR))
                            {
                                entidad_eliminar(proyectiles_jugador, i, &num_proyectiles_jugador);
                            }
                            for (int j = 0; j < num_entidades; j++)
                            {
                                if (colisiona_AABB(proyectiles_jugador[i], entidades[j]))
                                {
                                    proyectiles_jugador[i].vidas --;
                                    entidades[j].vidas --;
                                    if (proyectiles_jugador[i].vidas <= 0);
                                        entidad_eliminar(proyectiles_jugador, i, &num_proyectiles_jugador);
                                    if (entidades[j].vidas <= 0)
                                        entidad_eliminar(entidades, j, &num_entidades);
                                }
                            }
                        }

                        for (int i = 0; i < num_proyectiles_enemigos; i++)
                        {
                            if (!entidad_mover(&proyectiles_enemigo[i], ELIMINAR))
                            {
                                entidad_eliminar(proyectiles_enemigo, i, &num_proyectiles_enemigos);
                            }
                            if (danado == 0 && colisiona_AABB(jugador, proyectiles_enemigo[i]))
                            {
                                al_play_sample(sonidos[DANO_SONIDO], 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
                                danado = 1;
                                al_set_timer_count(framerate, 0);
                                jugador.vidas --;
                                proyectiles_enemigo[i].vidas--;
                                if (proyectiles_enemigo[i].vidas <= 0)
                                    entidad_eliminar(proyectiles_enemigo, i, &num_proyectiles_enemigos);
                            }
                        }

                        // Actualizando estado de las entidades inertes
                        for (int i = 0; i < num_inertes; i++)
                            if (!entidad_mover(&entidades_no_vivas[i], ELIMINAR))
                            {
                                entidades_no_vivas[i].x_pos = ANCHO - 10;
                            }

                    }
                    redibujar = 1;
                }
                else if (evento.timer.source == anim)
                {
                    if (mode == 0)
                    {
                        entidad_animar(&entidades_no_vivas[0]);
                        entidad_animar(&entidades_no_vivas[1]);

                    }
                    else if (pausa == 1)
                    {
                        al_set_timer_count(anim, al_get_timer_count(anim) - 1);
                    }
                    else if (mode == 1 && pausa == 0 && al_get_timer_count(anim) % 2 == 0)
                    {
                        entidad_animar(&jugador);
                        for (int i = 0; i < num_entidades; i++)
                            entidad_animar(&entidades[i]);
                    }
                }
                break;
            
            
            case ALLEGRO_EVENT_KEY_DOWN:
                if(evento.keyboard.keycode == ALLEGRO_KEY_W || evento.keyboard.keycode == ALLEGRO_KEY_UP)
                {
                    if (mode == 1)
                    {
                        jugador.y_vel += jugador.max_vel * -1;
                    }
                    else if (mode == 0 && ops>0)
                    {
                        ops--;
                        al_play_sample(sonidos[SELECCION_SONIDO], 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
                    }
                }
                else if (evento.keyboard.keycode == ALLEGRO_KEY_S || evento.keyboard.keycode == ALLEGRO_KEY_DOWN)
                {
                    if (mode == 1)
                    {
                        jugador.y_vel += jugador.max_vel;         
                    }
                    else if (mode == 0 && ops<2)
                    {
                        ops++;
                        al_play_sample(sonidos[SELECCION_SONIDO], 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
                    }      
                }
                if (evento.keyboard.keycode == ALLEGRO_KEY_A || evento.keyboard.keycode == ALLEGRO_KEY_LEFT) 
                {
                    if (mode == 1)
                    {
                        jugador.x_vel += jugador.max_vel * -1;
                    }
                }
                else if (evento.keyboard.keycode == ALLEGRO_KEY_D || evento.keyboard.keycode == ALLEGRO_KEY_RIGHT) 
                {
                    if (mode == 1)
                    {
                        jugador.x_vel += jugador.max_vel;                        
                    }
                }
                else if(evento.keyboard.keycode == ALLEGRO_KEY_SPACE)
                {
                    if (mode == 1 && pausa == 0)
                    {
                       entidad_crear(proyectiles_jugador, &num_proyectiles_jugador, PROYECTIL_JUGADOR, &jugador, NULL);
                        al_play_sample(sonidos[DISPARO_SONIDO], 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);

                    }


                }
                break;
            
            case ALLEGRO_EVENT_KEY_UP:
                
                if(evento.keyboard.keycode == ALLEGRO_KEY_W || evento.keyboard.keycode == ALLEGRO_KEY_UP)
                {
                    jugador.y_vel -= jugador.max_vel * -1;
                }
                else if (evento.keyboard.keycode == ALLEGRO_KEY_S || evento.keyboard.keycode == ALLEGRO_KEY_DOWN)
                {
                    jugador.y_vel -= jugador.max_vel;
                }
                if (evento.keyboard.keycode == ALLEGRO_KEY_A || evento.keyboard.keycode == ALLEGRO_KEY_LEFT)
                {
                    jugador.x_vel -= jugador.max_vel * -1;
                }
                else if (evento.keyboard.keycode == ALLEGRO_KEY_D || evento.keyboard.keycode == ALLEGRO_KEY_RIGHT) 
                {
                    jugador.x_vel -= jugador.max_vel;
                }
                break;
            
            case ALLEGRO_EVENT_KEY_CHAR:
                if (evento.keyboard.keycode == ALLEGRO_KEY_ENTER)
                {
                    if (mode == 0)
                    {
                        if (ops == 0)
                        {
                            num_inertes = 0;
                            num_entidades = 0;
                            num_proyectiles_enemigos = 0;
                            num_proyectiles_jugador = 0;
                            danado = 0;
                            mode = 1;
                            modo_inicializar(entidades_no_vivas, mode, &num_inertes, &jugador);
                        }
                        else if (ops == 1)
                        {
                            num_inertes = 0;
                            num_entidades = 0;
                            num_proyectiles_enemigos = 0;
                            num_proyectiles_jugador = 0;
                            mode = 2;
                        }
                        else if (ops == 2)
                            fin = 1;
                    }
                    else if (mode == 1 && pausa == 1)
                    {
                        num_inertes = 0;
                        num_entidades = 0;
                        num_proyectiles_enemigos = 0;
                        num_proyectiles_jugador = 0;
                        mode = 0;   
                        modo_inicializar(entidades_no_vivas, mode, &num_entidades, NULL);      
                    }

                }
                else if (evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                {
                    if (mode == 1)
                    {
                        pausa = !pausa;
                    }
                    else if (mode == 2) 
                    {
                        num_inertes = 0;
                        num_entidades = 0;
                        mode = 0;   
                        modo_inicializar(entidades_no_vivas, mode, &num_entidades, NULL);  
                    }        
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
