#include "utilidades.c"

// Esta función regresa un color de Allegro dependiendo de si la opción que recibe como parámetro coincide con el número del botón.
ALLEGRO_COLOR cambiar_color_boton(int opcion, int boton)
{
    ALLEGRO_COLOR color_select;
    if (opcion == boton)
        color_select= al_map_rgb(212, 197, 32);
    else
        color_select= al_map_rgb(212, 122, 32);

    return color_select;
}

//Una función en la que se incluyem todas las dibujos (Lunas y montañas) y los titulos de las opciones .
void dibujar_menu(struct Entidad entidades[], int *opcion, char *puntaje_mas_alto)
{
    // Dibujamos la luna.
    al_draw_scaled_bitmap(imagenes[LUNA_IMAGEN], 0, 0, al_get_bitmap_width(imagenes[LUNA_IMAGEN]), al_get_bitmap_height(imagenes[LUNA_IMAGEN]), 335, 95 ,200, 150, 0);
    al_draw_scaled_bitmap(imagenes[DRAGO_LUNA_IMAGEN], 0, 0, al_get_bitmap_width(imagenes[DRAGO_LUNA_IMAGEN]), al_get_bitmap_height(imagenes[DRAGO_LUNA_IMAGEN]), 365, 105 ,150, 114, 0);
    // Dibujamo las montañas.
    al_draw_scaled_bitmap(imagenes[FONDO_0_IMAGEN], 0, 0, al_get_bitmap_width(imagenes[FONDO_0_IMAGEN]), al_get_bitmap_height(imagenes[FONDO_0_IMAGEN]), 0, 0 ,ANCHO, ALTO, 0);
    al_draw_scaled_bitmap(imagenes[FONDO_1_IMAGEN], 0, 0, al_get_bitmap_width(imagenes[FONDO_1_IMAGEN]), al_get_bitmap_height(imagenes[FONDO_1_IMAGEN]), 0, 0 ,ANCHO, ALTO, 0);
    al_draw_scaled_bitmap(imagenes[FONDO_2_IMAGEN], 0, 0, al_get_bitmap_width(imagenes[FONDO_2_IMAGEN]), al_get_bitmap_height(imagenes[FONDO_2_IMAGEN]), 0, 0 ,ANCHO, 424, 0);

    //Dibujamos la torre según el índice en las entidades.
    entidad_dibujar(entidades[0]);
    al_draw_scaled_bitmap(imagenes[TORRE_0_IMAGEN], 0, 0, al_get_bitmap_width(imagenes[TORRE_0_IMAGEN]), al_get_bitmap_height(imagenes[TORRE_0_IMAGEN]), 15, 190 ,140, 290 , 0);

    //Dibujamos la torre según el índice en las entidades.
    entidad_dibujar(entidades[1]);


    al_draw_scaled_bitmap(imagenes[TORRE_0_IMAGEN], 0, 0, al_get_bitmap_width(imagenes[TORRE_0_IMAGEN]), al_get_bitmap_height(imagenes[TORRE_0_IMAGEN]), 485, 190 ,140, 290 , ALLEGRO_FLIP_HORIZONTAL);

    //Dibujamos el relieve de la última montaña
    al_draw_scaled_bitmap(imagenes[FONDO_3_IMAGEN], 0, 0, al_get_bitmap_width(imagenes[FONDO_3_IMAGEN]), al_get_bitmap_height(imagenes[FONDO_3_IMAGEN]), 0, 50 ,ANCHO, ALTO, 0);
    
    //Dibujamos el titulo 
    al_draw_text(fuentes[FUENTE_TITULO_80],al_map_rgb(150, 23, 6), ANCHO/2, ALTO-470, ALLEGRO_ALIGN_CENTER, "DRAAK BLIK");
    
    //Dibujamos las opciones con la funcion cambiar_color_boton que modifica el color de la opcion segun la seleccionemos o no
    al_draw_text(fuentes[FUENTE_TITULO_50],cambiar_color_boton(*opcion,0), ANCHO/2, ALTO-307, ALLEGRO_ALIGN_CENTER, "JUGAR");
    al_draw_text(fuentes[FUENTE_TITULO_50],cambiar_color_boton(*opcion,1), ANCHO/2, ALTO-207, ALLEGRO_ALIGN_CENTER, "CREDITOS");
    al_draw_text(fuentes[FUENTE_TITULO_50],cambiar_color_boton(*opcion,2), ANCHO/2, ALTO-107, ALLEGRO_ALIGN_CENTER, "SALIR");


    //Dibujamos el puntaje mas alto con la cadena puntaje_mas_alto
    al_draw_text(fuentes[FUENTE_15], al_map_rgb(150, 23, 6), ANCHO-15, ALTO-30, ALLEGRO_ALIGN_RIGHT, puntaje_mas_alto);
    al_draw_text(fuentes[FUENTE_15], al_map_rgb(150, 23, 6), 15, ALTO-30, ALLEGRO_ALIGN_LEFT, "UwUntu 2021");

}

//Una función en la que se incluyem todas las dibujos (Lunas y montañas) y los nombres 
void dibujar_creditos()
{
    //Dibujamos montañas y luna
    al_draw_scaled_bitmap(imagenes[WALL_IMAGEN], 0, 0, al_get_bitmap_width(imagenes[WALL_IMAGEN]), al_get_bitmap_height(imagenes[WALL_IMAGEN]), 0, 0 ,ANCHO, ALTO, 0);
    al_draw_scaled_bitmap(imagenes[LUNA_IMAGEN], 0, 0, al_get_bitmap_width(imagenes[LUNA_IMAGEN]), al_get_bitmap_height(imagenes[LUNA_IMAGEN]), 520, 50, 90, 90, 0);
    al_draw_scaled_bitmap(imagenes[MONT1_IMAGEN], 0, 0, al_get_bitmap_width(imagenes[MONT1_IMAGEN]), al_get_bitmap_height(imagenes[MONT1_IMAGEN]), 0, 0 ,ANCHO, ALTO, 0);
    al_draw_scaled_bitmap(imagenes[MONT2_IMAGEN], 0, 0, al_get_bitmap_width(imagenes[MONT2_IMAGEN]), al_get_bitmap_height(imagenes[MONT2_IMAGEN]), 0, 0 ,ANCHO, ALTO, 0);

    //Dibujamos nombre de los integrantes y del equipo 
    al_draw_text(fuentes[FUENTE_40], al_map_rgb(255, 255, 255), ANCHO/2, ALTO-440, ALLEGRO_ALIGN_CENTRE, "CREDITOS");
    al_draw_text(fuentes[FUENTE_20], al_map_rgb(255, 229, 153), ANCHO/2, ALTO-370, ALLEGRO_ALIGN_CENTRE, "HECHO POR:");
    al_draw_text(fuentes[FUENTE_15], al_map_rgb(255, 229, 153), ANCHO/2, ALTO-340, ALLEGRO_ALIGN_CENTRE, "UwUntu");
    al_draw_text(fuentes[FUENTE_15], al_map_rgb(255, 229, 153), ANCHO/2, ALTO-280, ALLEGRO_ALIGN_CENTRE, "ACOSTA DURAN LUNA FERNANDA");
    al_draw_text(fuentes[FUENTE_15], al_map_rgb(255, 229, 153), ANCHO/2, ALTO-250, ALLEGRO_ALIGN_CENTRE, "GALINDO MENA FRANCISCO");
    al_draw_text(fuentes[FUENTE_15], al_map_rgb(255, 229, 153), ANCHO/2, ALTO-220, ALLEGRO_ALIGN_CENTRE, "LOPEZ MORALES ZAIR");
    al_draw_text(fuentes[FUENTE_15], al_map_rgb(255, 229, 153), ANCHO/2, ALTO-190, ALLEGRO_ALIGN_CENTRE, "NAVARRO JUAREZ SERGIO DAMIAN");
    al_draw_text(fuentes[FUENTE_20], al_map_rgb(255, 229, 153), ANCHO/2, ALTO-130, ALLEGRO_ALIGN_CENTRE, "2021");
}
