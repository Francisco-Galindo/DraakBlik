#include "menuYCreditos.c"


#define ENEMIGOS_MAX 5
#define PROTECTILES_JUGADOR_MAX 64
#define PROTECTILES_ENEMIGO_MAX 64
#define INERTES_MAX 32

struct Entidad jugador;
struct Entidad enemigos[ENEMIGOS_MAX];
struct Entidad proyectiles_jugador[PROTECTILES_JUGADOR_MAX];
struct Entidad proyectiles_enemigo[PROTECTILES_ENEMIGO_MAX];
struct Entidad inertes[INERTES_MAX];

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


    int fin = 0, redibujar = 1, pausa = 1, modo = MENU, jugador_puntos = 0, num_enemigos = 0, num_inertes = 0, num_proyectiles_jugador = 0, num_proyectiles_enemigos = 0, opcion = 0, danado = 0;

    char cadena_vidas[15];
    char cadena_puntaje[15];
    char puntaje_mas_alto[16];

    // Cargando los elementos de Allegro
    if(!al_init())
    {
        printf("Todo ha muerto :(");
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
        printf("Error al cargar complemento de imagenes");
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

    // Creando los eventos que van a ser usados en el juego.
    eventos = al_create_event_queue();
    framerate = al_create_timer(1.0/FPS);
    anim = al_create_timer(0.075);
    puntaje = al_create_timer(1);
    al_register_event_source(eventos, al_get_display_event_source(disp));
    al_register_event_source(eventos, al_get_keyboard_event_source());
    al_register_event_source(eventos, al_get_timer_event_source(framerate));
    al_register_event_source(eventos, al_get_timer_event_source(anim));
    al_register_event_source(eventos, al_get_timer_event_source(puntaje));


    // Obteniendo el puntaje más alto para dibujarlo más adelante.
    int highscore = puntaje_mas_alto_obtener();
    sprintf(puntaje_mas_alto, "HIGHSCORE: %05i", highscore);

    // Inicializando el modo, que en este caso será el MENU.
    modo_inicializar(inertes, &num_inertes, modo, NULL);
 
    // Inicializando algunos de los timers.
    al_start_timer(framerate);
    al_start_timer(anim);
    al_flip_display();

   

    while (!fin)
    {
        // Dibujando pantalla correspondiente a cada modo.
        if (redibujar == 1 && al_event_queue_is_empty(eventos))
        {
            al_clear_to_color(color_fondo);


            if (modo == MENU)
            {
                dibujar_menu(inertes, &opcion, puntaje_mas_alto);
            }
            else if (modo == JUEGO)
            {
                al_draw_scaled_bitmap(imagenes[WALL_IMAGEN], 0, 0, al_get_bitmap_width(imagenes[WALL_IMAGEN]), al_get_bitmap_height(imagenes[WALL_IMAGEN]), 0, 0 ,ANCHO, ALTO, 0);

                al_draw_scaled_bitmap(imagenes[LUNA_IMAGEN], 0, 0, al_get_bitmap_width(imagenes[LUNA_IMAGEN]), al_get_bitmap_height(imagenes[LUNA_IMAGEN]), 520, 50, 90, 90, 0);

                for (int i = 0; i < num_inertes; i++)
                    entidad_dibujar(inertes[i]); 

                for (int i = 0; i < num_enemigos; i++)
                    entidad_dibujar(enemigos[i]);

                for (int i = 0; i < num_proyectiles_jugador; i++)                
                    entidad_dibujar(proyectiles_jugador[i]);

                for (int i = 0; i < num_proyectiles_enemigos; i++)
                    entidad_dibujar(proyectiles_enemigo[i]);
                

                // Hacer que el jugador parpadee si ha sido dañado.
                if ((danado == 0 || pausa == 1) || al_get_timer_count(anim) % 3 == 0)
                    entidad_dibujar(jugador);


                // Dibujar las vidas restantes y el puntaje actual
                sprintf(cadena_vidas, "Vidas: %02i", jugador.vidas);
                al_draw_text(fuentes[FUENTE_15], al_map_rgb(255, 255, 255), ANCHO-15, ALTO-30, ALLEGRO_ALIGN_RIGHT, cadena_vidas);
                sprintf(cadena_puntaje, "Puntaje: %05i", jugador_puntos);
                al_draw_text(fuentes[FUENTE_15], al_map_rgb(255, 255, 255), 15, ALTO-30, ALLEGRO_ALIGN_LEFT, cadena_puntaje);

                // Dibujar lo correspondiente a la pausa
                if (pausa == 1)
                {
                    if (jugador.vidas > 0)
                    {
                        //Siempre que estemos en Pausa se nos desplegara textos que explican los controles
                        al_draw_text(fuentes[FUENTE_60], al_map_rgb(245, 206, 66), ANCHO/2, 40, ALLEGRO_ALIGN_CENTRE, "PAUSA");
                        al_draw_text(fuentes[FUENTE_15], al_map_rgb(245, 206, 66), ANCHO/2, 390, ALLEGRO_ALIGN_CENTRE, "PRESIONA ENTER PARA IR AL MENU");
                        al_draw_scaled_bitmap(imagenes[CONTROL_0_IMAGEN], 0, 0, al_get_bitmap_width(imagenes[CONTROL_0_IMAGEN]), al_get_bitmap_height(imagenes[CONTROL_0_IMAGEN]), 110, 150 ,200, 150, 0);
                        al_draw_scaled_bitmap(imagenes[CONTROL_1_IMAGEN], 0, 0, al_get_bitmap_width(imagenes[CONTROL_1_IMAGEN]), al_get_bitmap_height(imagenes[CONTROL_0_IMAGEN]), 330, 150 ,200, 150, 0);
                        al_draw_text(fuentes[FUENTE_15], al_map_rgb(255, 255,255), ANCHO/2, 120, ALLEGRO_ALIGN_CENTRE, "Movimiento:");
                        al_draw_text(fuentes[FUENTE_15], al_map_rgb(255, 255,255), ANCHO/2, 320, ALLEGRO_ALIGN_CENTRE, "Disparo:");
                        al_draw_text(fuentes[FUENTE_15], al_map_rgb(255, 255,255), ANCHO/2, 355, ALLEGRO_ALIGN_CENTRE, "ESPACIO");
                    }
                    else
                    {
                        al_draw_text(fuentes[FUENTE_40], al_map_rgb(217, 4, 4), ANCHO/2, ALTO/2.5, ALLEGRO_ALIGN_CENTRE, "EL JUEGO HA");  
                        al_draw_text(fuentes[FUENTE_40], al_map_rgb(217, 4, 4), ANCHO/2, ALTO/2.5+40, ALLEGRO_ALIGN_CENTRE, "TERMINADO"); 

                        al_draw_text(fuentes[FUENTE_15], al_map_rgb(245, 206, 66), ANCHO/2, ALTO/1.5, ALLEGRO_ALIGN_CENTRE, "PRESIONA ENTER PARA IR AL MENU");                    
                    }
                }
            }
            else if (modo == CREDITOS)
            {
                dibujar_creditos();
            }

            al_flip_display();
            redibujar = 0;

        }

        al_wait_for_event(eventos, &evento);

        switch(evento.type)
        {
            case ALLEGRO_EVENT_TIMER:
            
                // Cada que un nuevo cuadro deba ser dibujado (indicado por el
                // timer "framerate"), realizar cálculos para actualizar el 
                // estado del juego, para luego cambiar el valor de la variable de dibujar a 1.
                if(evento.timer.source == framerate)
                {
                    if (modo == JUEGO && pausa == 0)
                    {
                        if(danado == 1 && al_get_timer_count(framerate) == FPS/0.75)
                        {
                            danado = 0;
                        }
                        // Aparición de nuevas entidades
                        if (num_enemigos < 5 && rand()%FPS == 3)
                        {
                            int tipo;
                            int numero_aleatorio = rand()%100;

                            // Eligiendo el tipo de entidad que se va a crear, dependiendo del numero aleatorio generado.
                            if (numero_aleatorio >= 0 && numero_aleatorio <= 49)
                                tipo = GARGOLA;
                            else if (numero_aleatorio >= 50 && numero_aleatorio <= 74)
                                tipo = MANTICORA;
                            else if (numero_aleatorio >= 75 && numero_aleatorio <= 96)
                                tipo = HIDRA;
                            else 
                                tipo = FENIX;

                            entidad_crear(enemigos, &num_enemigos, tipo, NULL, NULL); 
                        }

                        // Movimiento del jugador.
                        entidad_mover(&jugador, BLOQUEO);
            
                        // Actualización en el estado de los enemigos.
                        for (int i = 0; i < num_enemigos; i++)
                        {
                            entidad_mover(&enemigos[i], BLOQUEO);
                            if (enemigos[i].tipo == MANTICORA)
                            {
                                // Hacer que las mantícoras disparen en dirección del enemigo de manera aleatoria.
                                if (rand()%(FPS * 2) == 2)
                                {
                                    // Creando un proyectil que vaya hacia el jugador.
                                    entidad_crear(proyectiles_enemigo, &num_proyectiles_enemigos, MANTICORA_PROYECTIL, &enemigos[i], NULL);
                                    int indice = num_proyectiles_enemigos-1;
                                    entidad_perseguir(&proyectiles_enemigo[indice], jugador);
                                }
                                
                                // Las mantícoras van a rebotar en los bordes de la pantalla.
                                if (enemigos[i].y_pos >= ALTO-enemigos[i].alto || enemigos[i].y_pos <= 0)
                                    enemigos[i].y_vel *= -1;
                                
                                if (enemigos[i].x_pos >= ANCHO-enemigos[i].ancho || enemigos[i].x_pos <= 0)
                                    enemigos[i].x_vel *= -1;

                            }
                            else if (enemigos[i].tipo == GARGOLA)
                            {
                                // Las gárgolas dispararán aleatoriamente también.
                                if (rand()%(FPS*3) == 2)
                                    entidad_crear(proyectiles_enemigo, &num_proyectiles_enemigos, GARGOLA_PROYECTIL, &enemigos[i], NULL);

                                // Las gárgolas se moverán hacia el jugador.
                                entidad_perseguir(&enemigos[i], jugador);
                            }
                            else if (enemigos[i].tipo == HIDRA)
                            {

                                // La hidra va a ir disparando en momento aleatorios.
                                if (rand()%(int)(FPS*2.5) == 2)     
                                {
                                    // Creando tres proyectiles que se muevan en una manera similar a la de una escopeta.
                                    entidad_crear(proyectiles_enemigo, &num_proyectiles_enemigos, HIDRA_PROYECTIL, &enemigos[i], NULL);
                                    cambiar_angulo_movimiento(&proyectiles_enemigo[num_proyectiles_enemigos-1], (5 * PI)/6.0);

                                    entidad_crear(proyectiles_enemigo, &num_proyectiles_enemigos, HIDRA_PROYECTIL, &enemigos[i], NULL);
                                    cambiar_angulo_movimiento(&proyectiles_enemigo[num_proyectiles_enemigos-1], PI);

                                    entidad_crear(proyectiles_enemigo, &num_proyectiles_enemigos, HIDRA_PROYECTIL, &enemigos[i], NULL);
                                    cambiar_angulo_movimiento(&proyectiles_enemigo[num_proyectiles_enemigos-1], (7 * PI)/6.0);
                                }        

                                // La Hydra se moverá de arriba hacia abajo en la pantalla  .              
                                if (enemigos[i].y_pos == 0 && enemigos[i].y_vel <= 0)
                                    cambiar_angulo_movimiento(&enemigos[i], PI/2);
                                else if (enemigos[i].y_pos == ALTO-enemigos[i].alto && enemigos[i].y_vel >= 0)
                                    cambiar_angulo_movimiento(&enemigos[i], (3*PI)/2);
                            }
                            else if (enemigos[i].tipo == FENIX)
                            {

                                if  (rand()%(FPS*2) == 2)
                                {
                                    // Creando el disparo del fénix, el cual va a dirigirse al jugador.
                                    entidad_crear(proyectiles_enemigo, &num_proyectiles_enemigos, FENIX_PROYECTIL, &enemigos[i], NULL);
                                    entidad_perseguir(&proyectiles_enemigo[num_proyectiles_enemigos-1], jugador);
                                }

                                if (distancia_entre_entidades(enemigos[i], jugador) > (ALTO/2)+(enemigos[i].max_vel))
                                    entidad_perseguir(&enemigos[i], jugador);
                                else if (distancia_entre_entidades(enemigos[i], jugador) < (ALTO/2)-(enemigos[i].max_vel))
                                {
                                    entidad_perseguir(&enemigos[i], jugador);
                                    enemigos[i].x_vel *= -1;
                                    enemigos[i].y_vel *= -1;
                                }
                                else
                                {
                                    enemigos[i].x_vel *= 0;
                                    enemigos[i].y_vel *= 0 ;                              
                                }
                            }

                            // Colisiones de los enemigos con el jugador.
                            if (danado == 0 && entidades_reducir_vida_si_colisionan(&jugador, &enemigos[i], &danado))
                            {
                                al_set_timer_count(framerate, 0);
                                if (jugador.vidas <= 0)
                                {
                                    pausa = 1;
                                    al_stop_timer(puntaje);
                                    if (puntaje_mas_alto_obtener() < jugador_puntos)
                                        puntaje_mas_alto_guardar(jugador_puntos);
                                }
                                //Dibuja una explosión en las coordenas de donde estaba el enemigo.
                                entidad_crear(inertes,&num_inertes, GENERICO, NULL,imagenes[EXPLOSION_IMAGEN]);
                                inertes[num_inertes-1].x_pos = enemigos[i].x_pos;
                                inertes[num_inertes-1].y_pos = enemigos[i].y_pos;
                                
                                entidad_destruir_si_esta_muerta(enemigos, i, &num_enemigos, &jugador_puntos, &jugador);
                            }

                        }



                        // Actualización de estado de cada proyectil del jugador.
                        for (int i = 0; i < num_proyectiles_jugador; i++)
                        {
                            // Eliminando el proyectil si no
                            if (!entidad_mover(&proyectiles_jugador[i], ELIMINAR))
                                entidad_destruir(proyectiles_jugador, i, &num_proyectiles_jugador);
                            
                            for (int j = 0; j < num_enemigos; j++)
                            {
                                if (entidades_reducir_vida_si_colisionan(&proyectiles_jugador[i], &enemigos[j], NULL))
                                {
                                    entidad_destruir_si_esta_muerta(proyectiles_jugador, i, &num_proyectiles_jugador, NULL, NULL); 
                        
                                    // Darle una vida al jugador si mata a un Fénix.
                                    if(enemigos[j].tipo == FENIX)
                                    {
                                        jugador.vidas += 1;
                                        al_play_sample(sonidos[VIDA_SONIDO], 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
                                    }
                                    
                                    //Dibuja una explosión en las coordenas de donde estaba el protectil enemigo.
                                    entidad_crear(inertes,&num_inertes, GENERICO, NULL,imagenes[EXPLOSION_IMAGEN]);
                                    inertes[num_inertes-1].x_pos = enemigos[j].x_pos;
                                    inertes[num_inertes-1].y_pos = enemigos[j].y_pos;
                                    entidad_destruir_si_esta_muerta(enemigos, j, &num_enemigos, &jugador_puntos, &jugador);

                                }
                            }
                            for (int j = 0; j < num_proyectiles_enemigos; j++)
                            {
                                if (entidades_reducir_vida_si_colisionan(&proyectiles_jugador[i], &proyectiles_enemigo[j], NULL))
                                {
                                    entidad_destruir_si_esta_muerta(proyectiles_jugador, i, &num_proyectiles_jugador, NULL, NULL);
                                    entidad_destruir_si_esta_muerta( proyectiles_enemigo, j, &num_proyectiles_enemigos, NULL, NULL);
                                }
                            }
                        }



                        // Actualización de estado de cada proyectil enemigo.
                        for (int i = 0; i < num_proyectiles_enemigos; i++)
                        {
                            if (!entidad_mover(&proyectiles_enemigo[i], ELIMINAR))
                                entidad_destruir(proyectiles_enemigo, i, &num_proyectiles_enemigos);
            
                            if (danado == 0 && entidades_reducir_vida_si_colisionan(&jugador, &proyectiles_enemigo[i], &danado))
                            {
                                al_set_timer_count(framerate, 0);
                                if (jugador.vidas <= 0)
                                {
                                    pausa = 1;
                                    al_stop_timer(puntaje); 
                                    if (puntaje_mas_alto_obtener() < jugador_puntos)
                                        puntaje_mas_alto_guardar(jugador_puntos);                                 
                                }
                                entidad_destruir_si_esta_muerta(proyectiles_enemigo, i, &num_proyectiles_enemigos, NULL, NULL);
                            }
                        }



                        // Actualizando estado de las entidades inertes.
                        for (int i = 0; i < num_inertes; i++)
                            if (!entidad_mover(&inertes[i], ELIMINAR))
                            {
                                inertes[i].x_pos = ANCHO - 10;
                            }

                    }
                    else if (pausa == 1 && modo == JUEGO)
                    {
                        al_set_timer_count(framerate, al_get_timer_count(framerate) - 1);
                    }
                    redibujar = 1;
                }

                // Manejando el timer de animaciones.
                else if (evento.timer.source == anim)
                {
                    // Si la cuenta es par, y el modo es MENU, animar los fuegos, que corresponden a las dos primeras localidades del arreglo de entidades no vivas.
                    if (modo == MENU && al_get_timer_count(anim) % 2 == 0)
                    {
                        entidad_animar(&inertes[0]);
                        entidad_animar(&inertes[1]);

                    }
                    // Para el modo JUEGO
                    else if (modo == JUEGO && pausa == 0 && al_get_timer_count(anim) % 2 == 0)
                    {
                        // Si la cuenta  es par y el modo es JUEGO, animar entidades correspondientes.
                        if (al_get_timer_count(anim) % 2 == 0)
                        {
                            entidad_animar(&jugador);
                            for (int i = 0; i < num_enemigos; i++)
                                entidad_animar(&enemigos[i]);                           
                        }
                        if (al_get_timer_count(anim) % 3 == 0)
                        {
                            for(int i=0; i< 64; i++)
                            {
                                if(inertes[i].sprite == imagenes[EXPLOSION_IMAGEN])
                                    entidad_destruir(inertes, i,&num_inertes);

                            }
                        }

                    }
                }
                // Hacer que el puntaje del jugador suba cada que se active este timer
                else if (evento.timer.source == puntaje && pausa == 0)
                {
                    jugador_puntos ++;
                }
                break;
            
            
            case ALLEGRO_EVENT_KEY_DOWN:
                // Cuando se presione una tecla
                if(evento.keyboard.keycode == ALLEGRO_KEY_W || evento.keyboard.keycode == ALLEGRO_KEY_UP)
                {
                    // Cambiando velocidad del jugador en el modo JUEGO para ir hacia arriba.
                    if (modo == JUEGO)
                    {
                        jugador.y_vel += jugador.max_vel * -1;
                    }
                    // Cambiar de opción en el menú.
                    else if (modo == MENU && opcion>0)
                    {
                        opcion--;
                        al_play_sample(sonidos[SELECCION_SONIDO], 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
                    }
                }
                else if (evento.keyboard.keycode == ALLEGRO_KEY_S || evento.keyboard.keycode == ALLEGRO_KEY_DOWN)
                {
                    // Misma situación que con el caso anterior, pero hacia abajo
                    if (modo == JUEGO)
                    {
                        jugador.y_vel += jugador.max_vel;         
                    }
                    else if (modo == MENU && opcion<2)
                    {
                        opcion++;
                        al_play_sample(sonidos[SELECCION_SONIDO], 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
                    }      
                }
                if (evento.keyboard.keycode == ALLEGRO_KEY_A || evento.keyboard.keycode == ALLEGRO_KEY_LEFT) 
                {
                    if (modo ==JUEGO)
                    {
                        // Cambiar dirección del jugador a la izquierda
                        jugador.x_vel += jugador.max_vel * -1;
                    }
                }
                else if (evento.keyboard.keycode == ALLEGRO_KEY_D || evento.keyboard.keycode == ALLEGRO_KEY_RIGHT) 
                {
                    if (modo == JUEGO)
                    {
                        // Cambiar dirección del jugador a la derecha
                        jugador.x_vel += jugador.max_vel;                        
                    }
                }
                else if(evento.keyboard.keycode == ALLEGRO_KEY_SPACE)
                {
                    if (modo == JUEGO && pausa == 0)
                    {
                        entidad_crear(proyectiles_jugador, &num_proyectiles_jugador, JUGADOR_PROYECTIL, &jugador, NULL);
                        al_play_sample(sonidos[DISPARO_SONIDO], 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);

                    }


                }
                break;
            
            case ALLEGRO_EVENT_KEY_UP:
                // Cuando se deje de presionar una tecla, se restará la velocidad que se sumó antes en la dirección correspondiente
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
                    // Cuando se presione ENTER en el menú, se inicializará el modo correspondiente
                    if (modo == MENU)
                    {
                        al_stop_samples();
                        if (opcion == 0)
                        {
                            num_inertes = 0;
                            num_enemigos = 0;
                            num_proyectiles_enemigos = 0;
                            num_proyectiles_jugador = 0;
                            danado = 0;
                            modo = 1;
                            modo_inicializar(inertes, &num_inertes,  modo, &jugador);
                            al_start_timer(puntaje);
                        }
                        else if (opcion == 1)
                        {
                            
                            num_inertes = 0;
                            num_enemigos = 0;
                            num_proyectiles_enemigos = 0;
                            num_proyectiles_jugador = 0;
                            modo = 2;
                            modo_inicializar(inertes, &num_inertes, modo,  NULL);
                        }
                        else if (opcion == 2)
                            fin = 1;
                    }
                    // En el juego, se regresará el menú
                    else if (modo == JUEGO && pausa == 1)
                    {
                        al_stop_samples();
                        num_inertes = 0;
                        num_enemigos = 0;
                        num_proyectiles_enemigos = 0;
                        num_proyectiles_jugador = 0;
                        modo = 0;   
                        jugador_puntos = 0;
                        highscore = puntaje_mas_alto_obtener();
                        sprintf(puntaje_mas_alto, "HIGHSCORE: %05i", highscore);
                        modo_inicializar(inertes, &num_inertes, modo,  NULL);      
                    }
                    // También se regresará al menú des de CREDITOS
                    else if (modo == CREDITOS) 
                    {
                        al_stop_samples();
                        num_inertes = 0;
                        num_enemigos = 0;
                        num_proyectiles_enemigos = 0;
                        num_proyectiles_jugador = 0;
                        modo = 0;   
                        jugador_puntos = 0; 
                        modo_inicializar(inertes, &num_inertes, modo,  NULL);  
                    } 
                }
                else if (evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                {
                    // Pausar el juego
                    if (modo == JUEGO && jugador.vidas > 0)
                    {
                        pausa = !pausa;
                    }       
                }
                break;
            
            case ALLEGRO_EVENT_DISPLAY_SWITCH_OUT:
                if (modo == JUEGO) 
                    pausa = 1;
                break;
            case ALLEGRO_EVENT_DISPLAY_SWITCH_IN:
                redibujar = 1;
                break;
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                fin = 1;
                break;
        }


    }

    // Cerrando el programa.
    al_destroy_event_queue(eventos);
    al_destroy_display(disp);
    return 0;
}
