#ifndef ALPHA_CONSTANTS_HPP_INCLUDED
#define ALPHA_CONSTANTS_HPP_INCLUDED
//----------------- settings -----------------------


const unsigned char BACKGROUND_TRANSPARENCY = 255; // прозрачность пикселей фона

const unsigned char DEFAULT_RED          = 255; // цвет за картинкой
const unsigned char DEFAULT_GREEN        = 255;
const unsigned char DEFAULT_BLUE         = 255;
const unsigned char DEFAULT_TRANSPARENCY = 255;

const unsigned char TOP_BACKGROUND_RED          = DEFAULT_RED; // при замене верхних прорачных пикселей вне картинки
const unsigned char TOP_BACKGROUND_GREEN        = DEFAULT_GREEN;
const unsigned char TOP_BACKGROUND_BLUE         = DEFAULT_BLUE;
const unsigned char TOP_BACKGROUND_TRANSPARENCY = DEFAULT_TRANSPARENCY;

const unsigned char MERGED_TRANSPARENCY = 255; // прозрачность слитых пикселей

//--------------------------------------------------


// делает фоновые цвета очень яркими
//   ||
//   ||
//   \/
#define DEBUG_COLORS


//--------------------------------------------------


#ifdef DEBUG_COLORS

    #define DEFAULT_RED          0
    #define DEFAULT_GREEN        255
    #define DEFAULT_BLUE         0
    #define DEFAULT_TRANSPARENCY 255

    #define TOP_BACKGROUND_RED          255
    #define TOP_BACKGROUND_GREEN        0
    #define TOP_BACKGROUND_BLUE         0
    #define TOP_BACKGROUND_TRANSPARENCY 255

#endif


//--------------------------------------------------
#endif