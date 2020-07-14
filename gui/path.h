#ifndef PATH_HEADER_FILE
#define PATH_HEADER_FILE

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>


#define EXIT_COUNT 11
#define ENTRY_COUNT 11
#define AVENUE_COUNT 16

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

#endif