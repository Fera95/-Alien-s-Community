
#include "gui.h"

#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
int main()
{
    GUI_CONTEXT ctx;
    if (!init_gui(&ctx))
    {
        loop_gui(&ctx);
    };
    printf("Close main\n");
    return 0;
}