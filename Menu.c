#include <stdio.h>
#include <ctype.h>
#include <time.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "util.c"
#include "logicmenu.c"

int main()
{
    ALLEGRO_DISPLAY *disp;
    ALLEGRO_COLOR color_fondo;
    ALLEGRO_FONT *fuente70;
    ALLEGRO_FONT *fuente50;
    ALLEGRO_EVENT_QUEUE *eventos;
    ALLEGRO_EVENT evento;
    ALLEGRO_BITMAP *icono;
    ALLEGRO_TIMER *timer;

    ALLEGRO_BITMAP *fondo1;
    ALLEGRO_BITMAP *fondo2;
    ALLEGRO_BITMAP *fondo3;
    ALLEGRO_BITMAP *fondo4;
    ALLEGRO_BITMAP *fondo5;
    ALLEGRO_BITMAP *fondo6;
    ALLEGRO_BITMAP *torre1;
    ALLEGRO_BITMAP *torre2;
    ALLEGRO_BITMAP *luna;
    ALLEGRO_BITMAP *dragolun;
    ALLEGRO_BITMAP *fuego0;
    ALLEGRO_BITMAP *fuego1;
    ALLEGRO_BITMAP *fuego2;

    int fin = 0, redibujar = 1, pausa = 0,mov_fuego =0;
    

    if(!al_init())
    {
        printf("No se pudo cargar allegro");
        fin = 1;
    }
    
    if(!al_init_font_addon() || !al_init_ttf_addon())
    {
        printf("No se pudo cargar las fuentes");
        fin = 1;
    }
        if(!al_install_keyboard())
    {
        printf("No se instaló el teclado");
        fin = 1;
    }

    if(!al_init_image_addon())
    {
        printf("Imagenes error");
        fin = 1;
    }

    icono = al_load_bitmap("Imagenes/item.png");
    luna=al_load_bitmap("Imagenes/luna.png");
    dragolun=al_load_bitmap("Imagenes/dragolun.png");
    fondo1 = al_load_bitmap("Imagenes/pixil-frame-0 (14).png");
    fondo3= al_load_bitmap("Imagenes/pixil-frame-0 (13).png");
    fondo4= al_load_bitmap("Imagenes/pixil-frame-0 (17).png");
    fondo6= al_load_bitmap("Imagenes/pixil-frame-0 (18).png");
    fuego0= al_load_bitmap("Imagenes/fuego.png");
    fuego1= al_load_bitmap("Imagenes/fuego1.png");
    fuego2= al_load_bitmap("Imagenes/fuego2.png");
    torre1= al_load_bitmap("Imagenes/torre3.png");
    torre2= al_load_bitmap("Imagenes/torre4.png");
    fuente70 = al_load_font("Fuentes/PerfectDOSVGA437.ttf", 80, 0);
    fuente50 = al_load_font("Fuentes/PerfectDOSVGA437.ttf", 50, 0);
   
    struct Entidad fuego;
    fuego.sprite = fuego0;
    fuego.x_pos = 15;
    fuego.y_pos = 100;
    fuego.vel = 0;
    fuego.vidas = 3;
    fuego.alto = al_get_bitmap_width(fuego.sprite)*0.15;
    fuego.ancho = al_get_bitmap_height(fuego.sprite)*0.15;
    fuego.mov_arr = 0;
    fuego.mov_aba = 0;
    fuego.mov_der = 0;
    fuego.mov_izq = 0;

    struct Entidad fuego_izq;
    fuego_izq.sprite = fuego0;
    fuego_izq.x_pos = 480;
    fuego_izq.y_pos = 105;
    fuego_izq.vel = 0;
    fuego_izq.vidas = 3;
    fuego_izq.alto = al_get_bitmap_width(fuego_izq.sprite)*0.15;
    fuego_izq.ancho = al_get_bitmap_height(fuego_izq.sprite)*0.15;
    fuego_izq.mov_arr = 0;
    fuego_izq.mov_aba = 0;
    fuego_izq.mov_der = 0;
    fuego_izq.mov_izq = 0;
    
    if(!fuente70 || !fuente50)
    {
        printf("No se pudo cargar la fuente");
        fin = 1;
    }

    disp = al_create_display(640, 480);
    al_set_window_title(disp,"DRAAK BLIK");
    al_set_display_icon(disp, icono);

    timer = al_create_timer(0.15);
   
    color_fondo = al_map_rgb(1, 1, 31);
    al_clear_to_color(color_fondo);
    
    eventos = al_create_event_queue();

    al_register_event_source(eventos, al_get_display_event_source(disp));
    al_register_event_source(eventos, al_get_keyboard_event_source());

    al_register_event_source (eventos, al_get_timer_event_source (timer));
    //dibujar_entidad(fuego);
    al_flip_display();

    al_start_timer(timer);


    while(!fin)
    {
        if(redibujar == 1)
        {
            al_clear_to_color(color_fondo);
            al_draw_scaled_bitmap(luna, 0, 0, al_get_bitmap_width(luna), al_get_bitmap_height(luna), 335, 95 ,200, 150, 0);
            al_draw_scaled_bitmap(dragolun, 0, 0, al_get_bitmap_width(dragolun), al_get_bitmap_height(dragolun), 365, 105 ,150, 114, 0);
            
            al_draw_scaled_bitmap(fondo1, 0, 0, al_get_bitmap_width(fondo1), al_get_bitmap_height(fondo1), 0, 0 ,640, 480, 0);
            al_draw_scaled_bitmap(fondo3, 0, 0, al_get_bitmap_width(fondo3), al_get_bitmap_height(fondo3), 0, 0 ,640, 480, 0);
            al_draw_scaled_bitmap(fondo4, 0, 0, al_get_bitmap_width(fondo4), al_get_bitmap_height(fondo4), 0, 0 ,640, 424, 0);
           
            //Torre 1
            dibujar_entidad(fuego);
            al_draw_scaled_bitmap(torre1, 0, 0, al_get_bitmap_width(torre1), al_get_bitmap_height(torre1), 15, 190 ,140, 290 , 0);

            //Torre 2
            dibujar_entidad(fuego_izq);
            //al_draw_scaled_bitmap(fuego1, 0, 0, al_get_bitmap_width(fuego1), al_get_bitmap_height(fuego1), 500, 105 ,120, 136 , 0);
            //al_draw_scaled_bitmap(fuego0, 0, 0, al_get_bitmap_width(fuego0), al_get_bitmap_height(fuego0), 500, 105 ,120, 136 , 0);
            //al_draw_scaled_bitmap(fuego2, 0, 0, al_get_bitmap_width(fuego2), al_get_bitmap_height(fuego2), 500, 105 ,120, 136 , 0);

            al_draw_scaled_bitmap(torre2, 0, 0, al_get_bitmap_width(torre2), al_get_bitmap_height(torre2), 485, 190 ,140, 290 , 0);


            al_draw_scaled_bitmap(fondo6, 0, 0, al_get_bitmap_width(fondo6), al_get_bitmap_height(fondo6), 0, 50 ,640, 480, 0);
            
            al_draw_text(fuente70,al_map_rgb(150,23,6), 640/2, 480-470, ALLEGRO_ALIGN_CENTER, "DRAAK BLIK");
            
            al_draw_text(fuente50,color_evento(ops,1), 640/2, 480-280, ALLEGRO_ALIGN_CENTER, "JUGAR");
            al_draw_text(fuente50,color_evento(ops,2), 640/2, 480-180, ALLEGRO_ALIGN_CENTER, "CREDITOS");
            al_draw_text(fuente50,color_evento(ops,3), 640/2, 480-80, ALLEGRO_ALIGN_CENTER, "SALIR");

            
            al_flip_display();
            redibujar = 0;
        }

        al_wait_for_event(eventos, &evento);

        switch(evento.type)
        {
            case ALLEGRO_EVENT_TIMER:

                dibujar_fuego(&fuego, fuego0, fuego1, fuego2);
                ///////////////////////////////
                dibujar_fuego(&fuego_izq, fuego0, fuego1, fuego2);
                redibujar =1;

                break;
            case ALLEGRO_EVENT_KEY_CHAR:
                if(evento.keyboard.keycode == ALLEGRO_KEY_DOWN || evento.keyboard.keycode == ALLEGRO_KEY_S)
                {
                    if (ops<3)
                        ops++;
                    redibujar = 1;
                }
                else if (evento.keyboard.keycode == ALLEGRO_KEY_UP || evento.keyboard.keycode == ALLEGRO_KEY_W)
                {
                    if (ops>1)
                        ops--;
                    redibujar = 1;
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
