#ifndef COMMON_STRUCTS_HPP_INCLUDED
#define COMMON_STRUCTS_HPP_INCLUDED
//--------------------------------------------------


typedef struct Point {

    int x;
    int y;

} Point; const size_t POINT_SIZE = sizeof (Point);



typedef struct Pixel_Color24 {

    unsigned char red;
    unsigned char green;
    unsigned char blue;

} Pixel_Color24; const size_t PIXEL_COLOR24_SIZE = sizeof (Pixel_Color24);


typedef struct Pixel_Color32 {

    unsigned char red;
    unsigned char green;
    unsigned char blue;
    unsigned char transparency;

} Pixel_Color32; const size_t PIXEL_COLOR32_SIZE = sizeof (Pixel_Color32);


typedef struct Picture24 {

    Pixel_Color24* buffer;

    int width;
    int height;

} Picture24; const size_t PICTURE24_SIZE = sizeof (Picture24);


typedef struct Picture32 {

    Pixel_Color32* buffer;

    int width;
    int height;

} Picture_32; const size_t PICTURE32_SIZE = sizeof (Picture32);


//--------------------------------------------------
#endif