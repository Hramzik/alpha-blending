#ifndef COMMON_STRUCTS_HPP_INCLUDED
#define COMMON_STRUCTS_HPP_INCLUDED
//--------------------------------------------------


typedef struct Point {

    int x;
    int y;

} Point; const size_t POINT_SIZE = sizeof (Point);



typedef struct Pixel_color24 {

    unsigned char red;
    unsigned char green;
    unsigned char blue;

} Pixel_color24; const size_t PIXEL_COLOR24_SIZE = sizeof (Pixel_color24);


typedef struct Pixel_color32 {

    unsigned char red;
    unsigned char green;
    unsigned char blue;
    unsigned char transparency;

} Pixel_color32; const size_t PIXEL_COLOR32_SIZE = sizeof (Pixel_color32);


typedef struct Picture24 {

    Pixel_color24* buffer;

    int width;
    int height;

} Picture24; const size_t PICTURE24_SIZE = sizeof (Picture24);


typedef struct Picture32 {

    Pixel_color32* buffer;

    int width;
    int height;

} Picture_32; const size_t PICTURE32_SIZE = sizeof (Picture32);


//--------------------------------------------------
#endif