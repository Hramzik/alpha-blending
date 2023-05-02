

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


    return 0;
}

