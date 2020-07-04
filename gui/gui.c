#include "gui.h"

void must_init(bool test, const char *description)
{
    if (test)
        return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}
/**
*/
int init_gui(GUI_CONTEXT *ctx)
{
    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "keyboard");

    ctx->timer = al_create_timer(1.0 / 30.0);
    must_init(ctx->timer, "timer");

    ctx->queue = al_create_event_queue();
    must_init(ctx->queue, "queue");

    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);
    ctx->disp = al_create_display(975, 678);
    must_init(ctx->disp, "display");

    ctx->font = al_create_builtin_font();
    must_init(ctx->font, "font");

    set_background(ctx);

    must_init(al_init_primitives_addon(), "primitives");
    al_register_event_source(ctx->queue, al_get_keyboard_event_source());
    al_register_event_source(ctx->queue, al_get_display_event_source(ctx->disp));
    al_register_event_source(ctx->queue, al_get_timer_event_source(ctx->timer));
    ctx->done = false;
    ctx->redraw = true;
    printf("Init success\n");
    return 0;
}

int loop_gui(GUI_CONTEXT *ctx)
{
    /**
     */
    al_start_timer(ctx->timer);
    /**
      */
    while (1)
    {
        /**
         */
        al_wait_for_event(ctx->queue, &(ctx->event));
        /**
         */
        switch (ctx->event.type)
        {
        case ALLEGRO_EVENT_TIMER:
            /**
             * Logic here 
             */
            ctx->redraw = true;
            break;
        /**
         */
        case ALLEGRO_EVENT_KEY_DOWN:
        /**
         */
        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            ctx->done = true;
            break;
        }
        /**
         */
        if (ctx->done)
        {
            printf("Done\n");
            break;
        }
        /**
         */
        if (ctx->redraw && al_is_event_queue_empty(ctx->queue))
        {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_bitmap(ctx->background, 0, 0, 0);

            must_init(ctx->background, "background");
            ALLEGRO_BITMAP *alien = al_load_bitmap("assets/alfa.png");
            must_init(alien, "alien");
            al_draw_bitmap(alien, 100, 100, 0);
            al_draw_bitmap(alien, 180, 100, 1);
            al_draw_bitmap(alien, 220, 100, 2);



            al_draw_text(ctx->font, al_map_rgb(255, 255, 255), 0, 0, 0, "Hello world!");
            al_draw_filled_triangle(35, 350, 85, 375, 35, 400, al_map_rgb_f(0, 1, 0));
            al_draw_filled_rectangle(240, 260, 340, 340, al_map_rgba_f(0, 0, 0.5, 0.5));
            al_draw_circle(450, 370, 30, al_map_rgb_f(1, 0, 1), 2);
            al_draw_line(440, 110, 460, 210, al_map_rgb_f(1, 0, 0), 1);
            al_draw_line(500, 220, 570, 200, al_map_rgb_f(1, 1, 0), 1);
            al_flip_display();
            ctx->redraw = false;
        }
    }
    finalize_gui(ctx);
}

int finalize_gui(GUI_CONTEXT *ctx)
{
    al_destroy_font(ctx->font);
    al_destroy_display(ctx->disp);
    al_destroy_timer(ctx->timer);
    al_destroy_event_queue(ctx->queue);
    al_destroy_bitmap(ctx->background);
}

int set_background(GUI_CONTEXT *ctx)
{
    must_init(al_init_image_addon(), "image addon");
    ctx->background = al_load_bitmap("background.png");
    must_init(ctx->background, "background");
    return 0;
}

int add_character(GUI_CONTEXT *ctx, int id, int type, int dx, int dy){
    
}