#include "utilidades.c"

ALLEGRO_COLOR color_evento(int ops, int prue)
{
    ALLEGRO_COLOR color_select;
    if (ops == prue)
        color_select= al_map_rgb(212, 197, 32);
    else
        color_select= al_map_rgb(212, 122, 32);

    return color_select;
}

void dibujar_fuego (struct Entidad *entidad, ALLEGRO_BITMAP *imagen1, ALLEGRO_BITMAP *imagen2, ALLEGRO_BITMAP *imagen3)
{
    if(entidad->sprite == imagen1)
    {
        entidad->sprite = imagen2;
    }
    else if (entidad->sprite == imagen2)
    {
        entidad->sprite = imagen3;
    }
    else 
        entidad->sprite = imagen1;
}

void dibujar_menu(struct Entidad entidades[], int *ops)
{
    al_draw_scaled_bitmap(imagenes[LUNA_IMAGEN], 0, 0, al_get_bitmap_width(imagenes[LUNA_IMAGEN]), al_get_bitmap_height(imagenes[LUNA_IMAGEN]), 335, 95 ,200, 150, 0);
    al_draw_scaled_bitmap(imagenes[DRAGO_LUNA_IMAGEN], 0, 0, al_get_bitmap_width(imagenes[DRAGO_LUNA_IMAGEN]), al_get_bitmap_height(imagenes[DRAGO_LUNA_IMAGEN]), 365, 105 ,150, 114, 0);

    al_draw_scaled_bitmap(imagenes[FONDO_0_IMAGEN], 0, 0, al_get_bitmap_width(imagenes[FONDO_0_IMAGEN]), al_get_bitmap_height(imagenes[FONDO_0_IMAGEN]), 0, 0 ,640, 480, 0);
    al_draw_scaled_bitmap(imagenes[FONDO_1_IMAGEN], 0, 0, al_get_bitmap_width(imagenes[FONDO_1_IMAGEN]), al_get_bitmap_height(imagenes[FONDO_1_IMAGEN]), 0, 0 ,640, 480, 0);
    al_draw_scaled_bitmap(imagenes[FONDO_2_IMAGEN], 0, 0, al_get_bitmap_width(imagenes[FONDO_2_IMAGEN]), al_get_bitmap_height(imagenes[FONDO_2_IMAGEN]), 0, 0 ,640, 424, 0);

    //Torre 1
    dibujar_entidad(entidades[0]);
    al_draw_scaled_bitmap(imagenes[TORRE_0_IMAGEN], 0, 0, al_get_bitmap_width(imagenes[TORRE_0_IMAGEN]), al_get_bitmap_height(imagenes[TORRE_0_IMAGEN]), 15, 190 ,140, 290 , 0);

    //Torre 2
    dibujar_entidad(entidades[1]);


    al_draw_scaled_bitmap(imagenes[TORRE_1_IMAGEN], 0, 0, al_get_bitmap_width(imagenes[TORRE_1_IMAGEN]), al_get_bitmap_height(imagenes[TORRE_1_IMAGEN]), 485, 190 ,140, 290 , 0);


    al_draw_scaled_bitmap(imagenes[FONDO_3_IMAGEN], 0, 0, al_get_bitmap_width(imagenes[FONDO_3_IMAGEN]), al_get_bitmap_height(imagenes[FONDO_3_IMAGEN]), 0, 50 ,640, 480, 0);
    
    
    al_draw_text(fuentes[FUENTE_TITULO_80],al_map_rgb(150,23,6), 640/2, 480-470, ALLEGRO_ALIGN_CENTER, "DRAAK BLIK");
    

    al_draw_text(fuentes[FUENTE_TITULO_50],color_evento(*ops,0), 640/2, 480-280, ALLEGRO_ALIGN_CENTER, "JUGAR");
    al_draw_text(fuentes[FUENTE_TITULO_50],color_evento(*ops,1), 640/2, 480-180, ALLEGRO_ALIGN_CENTER, "CREDITOS");
    al_draw_text(fuentes[FUENTE_TITULO_50],color_evento(*ops,2), 640/2, 480-80, ALLEGRO_ALIGN_CENTER, "SALIR");
}


void dibujar_creditos()
{
    al_draw_scaled_bitmap(imagenes[WALL_IMAGEN], 0, 0, al_get_bitmap_width(imagenes[WALL_IMAGEN]), al_get_bitmap_height(imagenes[WALL_IMAGEN]), 0, 0 ,640, 480, 0);
    al_draw_scaled_bitmap(imagenes[LUNA_IMAGEN], 0, 0, al_get_bitmap_width(imagenes[LUNA_IMAGEN]), al_get_bitmap_height(imagenes[LUNA_IMAGEN]), 520, 50, 90, 90, 0);
    al_draw_scaled_bitmap(imagenes[MONT1_IMAGEN], 0, 0, al_get_bitmap_width(imagenes[MONT1_IMAGEN]), al_get_bitmap_height(imagenes[MONT1_IMAGEN]), 0, 0 ,640, 480, 0);
    al_draw_scaled_bitmap(imagenes[MONT2_IMAGEN], 0, 0, al_get_bitmap_width(imagenes[MONT2_IMAGEN]), al_get_bitmap_height(imagenes[MONT2_IMAGEN]), 0, 0 ,640, 480, 0);
    al_draw_text(fuentes[FUENTE_40], al_map_rgb(255, 255, 255), 640/2, 480-440, ALLEGRO_ALIGN_CENTRE, "CREDITOS");
    al_draw_text(fuentes[FUENTE_20], al_map_rgb(255, 229, 153), 640/2, 480-370, ALLEGRO_ALIGN_CENTRE, "HECHO POR:");
    al_draw_text(fuentes[FUENTE_15], al_map_rgb(255, 229, 153), 640/2, 480-340, ALLEGRO_ALIGN_CENTRE, "UwUntu");
    al_draw_text(fuentes[FUENTE_15], al_map_rgb(255, 229, 153), 640/2, 480-280, ALLEGRO_ALIGN_CENTRE, "ACOSTA DURAN LUNA FERNANDA");
    al_draw_text(fuentes[FUENTE_15], al_map_rgb(255, 229, 153), 640/2, 480-250, ALLEGRO_ALIGN_CENTRE, "GALINDO MENA FRANCISCO");
    al_draw_text(fuentes[FUENTE_15], al_map_rgb(255, 229, 153), 640/2, 480-220, ALLEGRO_ALIGN_CENTRE, "LOPEZ MORALES ZAIR");
    al_draw_text(fuentes[FUENTE_15], al_map_rgb(255, 229, 153), 640/2, 480-190, ALLEGRO_ALIGN_CENTRE, "NAVARRO JUAREZ SERGIO DAMIAN");
    al_draw_text(fuentes[FUENTE_20], al_map_rgb(255, 229, 153), 640/2, 480-130, ALLEGRO_ALIGN_CENTRE, "2021");
}
