
#include "juego.c"

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
    ALLEGRO_TIMER *puntaje = NULL;

    int fin = 0, redibujar = 1, pausa = 1, mode = 0, jugador_puntos = 0, num_entidades = 0, num_inertes = 0, num_proyectiles_jugador = 0, num_proyectiles_enemigos = 0, ops = 0, danado = 0;

    char cadena_vidas[15];
    char cadena_puntaje[15];

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
    puntaje = al_create_timer(1);
    al_register_event_source(eventos, al_get_display_event_source(disp));
    al_register_event_source(eventos, al_get_keyboard_event_source());
    al_register_event_source(eventos, al_get_timer_event_source(framerate));
    al_register_event_source(eventos, al_get_timer_event_source(anim));
    al_register_event_source(eventos, al_get_timer_event_source(puntaje));

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
                al_draw_scaled_bitmap(imagenes[WALL_IMAGEN], 0, 0, al_get_bitmap_width(imagenes[WALL_IMAGEN]), al_get_bitmap_height(imagenes[WALL_IMAGEN]), 0, 0 ,ANCHO, ALTO, 0);

                al_draw_scaled_bitmap(imagenes[LUNA_IMAGEN], 0, 0, al_get_bitmap_width(imagenes[LUNA_IMAGEN]), al_get_bitmap_height(imagenes[LUNA_IMAGEN]), 520, 50, 90, 90, 0);

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


                sprintf(cadena_vidas, "Vidas: %02i", jugador.vidas);
                al_draw_text(fuentes[FUENTE_15], al_map_rgb(255, 255, 255), ANCHO-15, ALTO-30, ALLEGRO_ALIGN_RIGHT, cadena_vidas);
                sprintf(cadena_puntaje, "Puntaje: %05i", jugador_puntos);
                al_draw_text(fuentes[FUENTE_15], al_map_rgb(255, 255, 255), 15, ALTO-30, ALLEGRO_ALIGN_LEFT, cadena_puntaje);

                if (pausa == 1)
                {
                    if (jugador.vidas > 0)
                    {
                        al_draw_text(fuentes[FUENTE_60], al_map_rgb(255, 255, 255), ANCHO/2, ALTO/2.5, ALLEGRO_ALIGN_CENTRE, "PAUSA");
                        al_draw_text(fuentes[FUENTE_15], al_map_rgb(255, 255, 255), ANCHO/2, ALTO/1.5, ALLEGRO_ALIGN_CENTRE, "PRESIONA ENTER PARA IR AL MENU");
                    }
                    else
                    {
                        al_draw_text(fuentes[FUENTE_40], al_map_rgb(255, 255, 255), ANCHO/2, ALTO/2.5, ALLEGRO_ALIGN_CENTRE, "EL JUEGO HA");  
                        al_draw_text(fuentes[FUENTE_40], al_map_rgb(255, 255, 255), ANCHO/2, ALTO/2.5+40, ALLEGRO_ALIGN_CENTRE, "TERMINADO"); 

                        al_draw_text(fuentes[FUENTE_15], al_map_rgb(255, 255, 255), ANCHO/2, ALTO/1.5, ALLEGRO_ALIGN_CENTRE, "PRESIONA ENTER PARA IR AL MENU");                    
                    }
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
                        if (num_entidades < 5 && rand()%FPS == 3)
                        {
                            int tipo;
                            int numero_aleatorio = rand()%16;
                            if (numero_aleatorio >= 0 && numero_aleatorio <= 7)
                                tipo = GARGOLA;
                            else if (numero_aleatorio >= 8 && numero_aleatorio <= 11)
                                tipo = MANTICORA;
                            else if (numero_aleatorio >= 12 && numero_aleatorio <= 13)
                                tipo = HYDRA;
                            else 
                                tipo = FENIX;

                            entidad_crear(entidades, &num_entidades, tipo, NULL, NULL); 
                            printf("%i\n", num_entidades);
                        }

                        // Movimiento del jugador
                        entidad_mover(&jugador, BLOQUEO);
            
                        // Actualización en el estado de los enemigos
                        for (int i = 0; i < num_entidades; i++)
                        {
                            entidad_mover(&entidades[i], BLOQUEO);
                            // Hacer que las manticoras disparen en dirección del enemigo de manera aleatoria
                            if (entidades[i].tipo == MANTICORA)
                            {
                                if (rand()%(FPS * 2) == 2)
                                {
                                    // Creando un proyectil que vaya hacia el jugador
                                    entidad_crear(proyectiles_enemigo, &num_proyectiles_enemigos, PROYECTIL_MANTICORA, &entidades[i], NULL);
                                    int indice = num_proyectiles_enemigos-1;
                                    girar_hacia_entidad(&proyectiles_enemigo[indice], jugador);
                                }
                                cambiar_angulo_movimiento(&entidades[i], PI/2.0);


                            }
                            // Las gárgolas dispararán aleatoriamente también
                            else if (entidades[i].tipo == GARGOLA)
                            {
                                if (rand()%FPS*1 == 2)
                                    entidad_crear(proyectiles_enemigo, &num_proyectiles_enemigos, PROYECTIL_GARGOLA, &entidades[i], NULL);
                                girar_hacia_entidad(&entidades[i], jugador);
                            }
                            else if (entidades[i].tipo == HYDRA)
                            {

                                if (rand()%(int)(FPS*2.5) == 2)     
                                {
                                    // Creando tres proyectiles que se muevan en una manera similar a la de una escopeta
                                    entidad_crear(proyectiles_enemigo, &num_proyectiles_enemigos, PROYECTIL_HYDRA, &entidades[i], NULL);
                                    cambiar_angulo_movimiento(&proyectiles_enemigo[num_proyectiles_enemigos-1], PI/6.0);

                                    entidad_crear(proyectiles_enemigo, &num_proyectiles_enemigos, PROYECTIL_HYDRA, &entidades[i], NULL);
                                    cambiar_angulo_movimiento(&proyectiles_enemigo[num_proyectiles_enemigos-1], 0);

                                    entidad_crear(proyectiles_enemigo, &num_proyectiles_enemigos, PROYECTIL_HYDRA, &entidades[i], NULL);
                                    cambiar_angulo_movimiento(&proyectiles_enemigo[num_proyectiles_enemigos-1], PI/-6.0);
                                }                         
                                if (entidades[i].y_pos == 0 && entidades[i].y_vel <= 0)
                                {
                                    cambiar_angulo_movimiento(&entidades[i], PI/2);
                                    printf("CUM: %f\n", entidades[i].y_vel);
                                }
                                else if (entidades[i].y_pos == ALTO-entidades[i].alto && entidades[i].y_vel >= 0)
                                {
                                    cambiar_angulo_movimiento(&entidades[i], (3*PI)/2);

                                    printf("LOL: %f\n", entidades[i].y_vel);
                                }
                            }
                            else if (entidades[i].tipo == FENIX)
                            {
                                if (1)
                                {
                                    //asdf
                                }
                                girar_hacia_entidad(&entidades[i], jugador);
                                if (distancia_hasta(entidades[i], jugador) > 300)
                                {
                                    entidades[i].x_vel *= -1;
                                    entidades[i].y_vel *= -1;
                                }
                            }



                            if (danado == 0 && checar_colisiones(&jugador, &entidades[i], &danado))
                            {
                                al_set_timer_count(framerate, 0);
                                if (jugador.vidas <= 0)
                                {
                                    pausa = 1;
                                    al_stop_timer(puntaje);
                                }
                                if (entidades[i].vidas <= 0)
                                    entidad_eliminar(entidades, i, &num_entidades);
                            }

                        }


                        // Actualización de estado de cada proyectil del jugador
                        for (int i = 0; i < num_proyectiles_jugador; i++)
                        {
                            if (!entidad_mover(&proyectiles_jugador[i], ELIMINAR))
                                entidad_eliminar(proyectiles_jugador, i, &num_proyectiles_jugador);
                            
                            for (int j = 0; j < num_entidades; j++)
                            {
                                if (checar_colisiones(&proyectiles_jugador[i], &entidades[j], NULL))
                                {
                                    if (proyectiles_jugador[i].vidas <= 0);
                                        entidad_eliminar(proyectiles_jugador, i, &num_proyectiles_jugador);
                                    if (entidades[j].vidas <= 0)
                                        entidad_eliminar(entidades, j, &num_entidades);
                                }
                            }
                            for (int j = 0; j < num_proyectiles_enemigos; j++)
                            {
                                if (checar_colisiones(&proyectiles_jugador[i], &proyectiles_enemigo[j], NULL))
                                {
                                    if (proyectiles_jugador[i].vidas <= 0);
                                        entidad_eliminar(proyectiles_jugador, i, &num_proyectiles_jugador);
                                    if (proyectiles_enemigo[j].vidas <= 0)
                                        entidad_eliminar(proyectiles_enemigo, j, &num_proyectiles_enemigos);
                                }
                            }
                        }

                        // Actualización de estado de cada proyectil enemigo
                        for (int i = 0; i < num_proyectiles_enemigos; i++)
                        {
                            if (!entidad_mover(&proyectiles_enemigo[i], ELIMINAR))
                                entidad_eliminar(proyectiles_enemigo, i, &num_proyectiles_enemigos);
            
                            if (danado == 0 && checar_colisiones(&jugador, &proyectiles_enemigo[i], &danado))
                            {
                                al_set_timer_count(framerate, 0);
                                if (jugador.vidas <= 0)
                                {
                                    pausa = 1;
                                    al_stop_timer(puntaje);                                    
                                }
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
                    else if (pausa == 1 && mode == 1)
                    {
                        al_set_timer_count(framerate, al_get_timer_count(framerate) - 1);
                    }
                    redibujar = 1;
                }
                else if (evento.timer.source == anim)
                {
                    if (mode == 0 && al_get_timer_count(anim) % 2 == 0)
                    {
                        entidad_animar(&entidades_no_vivas[0]);
                        entidad_animar(&entidades_no_vivas[1]);

                    }
                    else if (mode == 1 && pausa == 0 && al_get_timer_count(anim) % 2 == 0)
                    {
                        entidad_animar(&jugador);
                        for (int i = 0; i < num_entidades; i++)
                            entidad_animar(&entidades[i]);
                    }
                }
                else if (evento.timer.source == puntaje && pausa == 0)
                {
                    jugador_puntos ++;
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
                if (evento.keyboard.keycode == ALLEGRO_KEY_ENTER || evento.keyboard.keycode == ALLEGRO_KEY_PAD_ENTER)
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
                            al_start_timer(puntaje);
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
                        jugador_puntos = 0;
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
        }


    }

    al_destroy_event_queue(eventos);
    al_destroy_display(disp);
    return 0;
}
