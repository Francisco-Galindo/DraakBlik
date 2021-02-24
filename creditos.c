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

int main ()
{

    ALLEGRO_DISPLAY *disp;
    ALLEGRO_COLOR color_fondo;
    ALLEGRO_EVENT_QUEUE *eventos;
    ALLEGRO_EVENT evento;
    ALLEGRO_BITMAP *icono;
    ALLEGRO_BITMAP *wall;
    ALLEGRO_BITMAP *mont1;
    ALLEGRO_BITMAP *mont2;
    ALLEGRO_BITMAP *luna;
    ALLEGRO_FONT *fuente60;
    ALLEGRO_FONT *fuente40;
    ALLEGRO_FONT *fuente30;

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

    icono = al_load_bitmap("Imagenes/item.png");
    wall = al_load_bitmap("Imagenes/fondo.png");
    mont1 = al_load_bitmap("Imagenes/mnt5.png");
    mont2 = al_load_bitmap("Imagenes/mnt6.png");
    luna = al_load_bitmap("Imagenes/luna.png");

    fuente60 = al_load_font("Fuentes/PressStart2P.ttf", 40, 0);
    fuente40 = al_load_font("Fuentes/PressStart2P.ttf", 20, 0);
    fuente30 = al_load_font("Fuentes/PressStart2P.ttf", 15, 0);

    if(!fuente60 || !fuente40)
    {
        printf("No se pudo cargar fuente");
        fin = 1;
    }

    disp = al_create_display(640,480);
    al_set_window_title(disp, "DRAAK BLIK");
    al_set_display_icon(disp, icono);

    color_fondo = al_map_rgb(20,20,20);
    al_clear_to_color(color_fondo);

    eventos = al_create_event_queue();

    al_register_event_source(eventos, al_get_display_event_source(disp));
    al_register_event_source(eventos, al_get_keyboard_event_source());

    al_flip_display();


    while (!fin)
    {
        al_draw_scaled_bitmap(wall, 0, 0, al_get_bitmap_width(wall), al_get_bitmap_height(wall), 0, 0 ,640, 480, 0);
        al_draw_scaled_bitmap(luna, 0, 0, al_get_bitmap_width(luna), al_get_bitmap_height(luna), 520, 50, 90, 90, 0);
        al_draw_scaled_bitmap(mont1, 0, 0, al_get_bitmap_width(mont1), al_get_bitmap_height(mont1), 0, 0 ,640, 480, 0);
        al_draw_scaled_bitmap(mont2, 0, 0, al_get_bitmap_width(mont2), al_get_bitmap_height(mont2), 0, 0 ,640, 480, 0);
        al_draw_text(fuente60, al_map_rgb(255, 255, 255), 640/2, 480-440, ALLEGRO_ALIGN_CENTRE, "CREDITOS");
        al_draw_text(fuente40, al_map_rgb(255, 229, 153), 640/2, 480-370, ALLEGRO_ALIGN_CENTRE, "HECHO POR:");
        al_draw_text(fuente30, al_map_rgb(255, 229, 153), 640/2, 480-340, ALLEGRO_ALIGN_CENTRE, "UwUntu");
        al_draw_text(fuente30, al_map_rgb(255, 229, 153), 640/2, 480-280, ALLEGRO_ALIGN_CENTRE, "ACOSTA DURAN LUNA FERNANDA");
        al_draw_text(fuente30, al_map_rgb(255, 229, 153), 640/2, 480-250, ALLEGRO_ALIGN_CENTRE, "GALINDO MENA FRANCISCO");
        al_draw_text(fuente30, al_map_rgb(255, 229, 153), 640/2, 480-220, ALLEGRO_ALIGN_CENTRE, "LOPEZ MORALES ZAIR");
        al_draw_text(fuente30, al_map_rgb(255, 229, 153), 640/2, 480-190, ALLEGRO_ALIGN_CENTRE, "NAVARRO JUAREZ SERGIO DAMIAN");
        al_draw_text(fuente40, al_map_rgb(255, 229, 153), 640/2, 480-130, ALLEGRO_ALIGN_CENTRE, "2021");


        al_flip_display();

        al_wait_for_event(eventos, &evento);

        switch(evento.type)
        {
            //Cambiar a: Para regresar al men�
            /*case ALLEGRO_EVENT_KEY_CHAR:
                if(evento.keyboard.keycode == ALLEGRO_KEY_SPACE)
                {
                    pausa = !pausa;
                }
                break;*/

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
