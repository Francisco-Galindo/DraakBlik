#include "menuYCreditos.c"


void girar_hacia_entidad(struct Entidad *entidad_a_mover, struct Entidad entidad_destino)
{
    // Vamos a imaginar la velocidad en cada eje de una entidad como los componentes de un vector, así, sabiendo la posición de una entidad de destino, podemos hacer que el vector de movimiento apunte hacia allá

    // Calculando la distancia entre el proyectil y el jugador
    float dis_x = (entidad_a_mover->x_pos + 
                   (entidad_a_mover->ancho / 2)) - 
                    (entidad_destino.x_pos + (entidad_destino.ancho / 2));
    float dis_y = (entidad_a_mover->y_pos + 
                   (entidad_a_mover->alto / 2)) - 
                   (entidad_destino.y_pos + (entidad_destino.alto / 2));
    double dis_total = pow((dis_x * dis_x) + (dis_y * dis_y), 0.5);

    // Modificando la magnitud del vector para que sea igual a la velocidad máxima de movimiento de la entidad a mover
    double const_proporcionalidad = dis_total/entidad_a_mover->max_vel;
    entidad_a_mover->x_vel = dis_x/const_proporcionalidad;
    entidad_a_mover->y_vel = dis_y/const_proporcionalidad;
}

double distancia_hasta(struct Entidad entidad_uno, struct Entidad entidad_dos)
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

int checar_colisiones(struct Entidad *entidad_uno, struct Entidad *entidad_dos, int *marcar_como_danado)
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

