#ifndef ALPHA_STRUCTS_HPP_INCLUDED
#define ALPHA_STRUCTS_HPP_INCLUDED
//--------------------------------------------------


#include "alpha.hpp"


//--------------------------------------------------


typedef struct Buffer_saver3 {

    Pixel_Color32* top;
    Pixel_Color24* bottom;
    Pixel_Color32* result;

} Buffer_saver3; const size_t BUFFER_SAVER3_SIZE = sizeof (Buffer_saver3);


//--------------------------------------------------
#endif