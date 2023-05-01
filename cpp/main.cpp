

//--------------------------------------------------

#include "../hpp/graphics/graphics.hpp"

//--------------------------------------------------


int main (void) {

    initialize_sdl ();
    Image_Mixer* mixer = mixer_ctor ();


    mixer_load_media (mixer);
    mixer_work (mixer);


    mixer_dtor (mixer);
    SDL_Quit   ();


    printf ("goodbye!\n");

    Pixel_Color32 clr = { .red = 255, .green = 200, .blue = 100, .transparency = 50};
    unsigned char* test = (unsigned char*) &clr;
    printf ("debug: passed point %d %d %d %d\n", (*test), *(test+1), *(test+2), *(test+3));


    return 0;
}

