#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

typedef struct PATH
{
    // Bloqueado
    int blocked;
    // COORDENADAS
    float x, y;
    int width, height;
    // IMAGE
    ALLEGRO_BITMAP * image;
    // CONTEXT
    // GUI_CONTEXT *ctx;
    
} PATH;
