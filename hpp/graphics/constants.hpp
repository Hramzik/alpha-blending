#ifndef GRAPHICS_CONSTANTS_INCLUDED
#define GRAPHICS_CONSTANTS_INCLUDED
//--------------------------------------------------


#include "graphics.hpp"


//--------------------------------------------------


const Uint32 MIXER_OUTPUT_FLAGS = SDL_WINDOW_SHOWN | /*SDL_WINDOW_BORDERLESS | SDL_WINDOW_SKIP_TASKBAR |
    SDL_WINDOW_INPUT_GRABBED*/ SDL_WINDOW_ALWAYS_ON_TOP;


const size_t SIZE_T_SIZE = sizeof (size_t);
const size_t CHAR_SIZE   = sizeof (char);


const size_t FILE_SIZE_OFFSET = 0x02; // смещение информации о размере файла относительно начала в bmp файле
const size_t FILE_SIZE_LEN    = 4;    // размер   информации о размере файла                     в bmp файле

const size_t DATA_OFFSET_OFFSET = 0x0A;
const size_t DATA_OFFET_LEN     = 4;

const size_t SIGNATURE_OFFSET = 0x00;
const size_t SIGNATURE_LEN    = 2;
const size_t SIGNATURE_ETALON = 0x4D42; // bmp signature


const size_t HEADER_SIZE_OFFSET  = 0x0E;
const size_t HEADER_SIZE_LEN     = 4;
const size_t HEADER_SIZE_ETALON1 = 40;  // bmp3
const size_t HEADER_SIZE_ETALON2 = 108; // bmp4
const size_t HEADER_SIZE_ETALON3 = 124; // bmp5

const size_t WIDTH_OFFSET  = 0x12;
const size_t WIDTH_LEN     = 4;

const size_t HEIGHT_OFFSET  = 0x16;
const size_t HEIGHT_LEN     = 4;


const int HORIZONTAL_MOVE_SMALL_STEP = 1; // in pixels, arrows
const int VERTICAL_MOVE_SMALL_STEP   = 1;
const int HORIZONTAL_MOVE_BIG_STEP = 15;   // in pixels, wasd
const int VERTICAL_MOVE_BIG_STEP   = 15;

#define DEFAULT_SAVING_PATH "work/result.png"


const size_t DEFAULT_WINDOW_WIDTH  = 800;
const size_t DEFAULT_WINDOW_HEIGHT = 800;


//--------------------------------------------------
#endif