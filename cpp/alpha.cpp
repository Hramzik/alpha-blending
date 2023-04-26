

//--------------------------------------------------


#include "../hpp/alpha/alpha.hpp"


//--------------------------------------------------


#include "../lib/logs.hpp"


//--------------------------------------------------


Return_code merge_pictures (Picture32* result, Picture32* top, Picture24* bottom, Point offset) { // point = top-left

    if (!result) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!top)    { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!bottom) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    Buffer_saver3* saved_buffers = save_buffers (top, bottom, result);


    int current_line = my_min (offset.y, 0);

    copy_top_upper    (result, &current_line, top, offset);
    fill_void_upper   (result, &current_line);
    copy_bottom_upper (result, &current_line, top, bottom, offset);

    merge_top_with_bottom (result, &current_line, top, bottom, offset);

    copy_bottom_lower (result, &current_line, bottom, offset);
    fill_void_lower   (result, &current_line,         offset);
    copy_top_lower    (result, &current_line, top,    offset);


    restore_buffers (top, bottom, result, saved_buffers);


    return SUCCESS;
}


Return_code copy_top_upper (Picture32* result, int* line, Picture32* top, Point offset) {

    if (!result) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!line)   { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!top)    { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    int last_line = my_min (0, top->height + offset.y);

    while (*line < last_line) {

        copy_top_line (result, top, offset);
        *line += 1;
    }


    return SUCCESS;
}


Return_code fill_void_upper (Picture32* result, int* line) {

    if (!result) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!line)   { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    while (*line < 0) {

        load_line_default (result);
        *line += 1;
    }


    return SUCCESS;
}


Return_code copy_bottom_upper (Picture32* result, int* line, Picture32* top, Picture24* bottom, Point offset) {

    if (!result) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!line)   { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!top)    { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!bottom) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    int last_line = my_min (offset.y, bottom->height);

    while (*line < last_line) {

        copy_bottom_line (result, bottom, offset);
        *line += 1;
    }


    return SUCCESS;
}


Return_code merge_top_with_bottom (Picture32* result, int* line, Picture32* top, Picture24* bottom, Point offset) {

    if (!result) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!line)   { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!top)    { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!bottom) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    int last_line = my_min (offset.y + top->height, bottom->height);

    while (*line < last_line) {

        merge_line (result, top, bottom, offset);
        *line += 1;
    }


    return SUCCESS;
}


Return_code copy_bottom_lower (Picture32* result, int* line, Picture24* bottom, Point offset) {

    if (!result) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!line)   { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!bottom) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    while (*line < bottom->height) {

        copy_bottom_line (result, bottom, offset);
        *line += 1;
    }


    return SUCCESS;
}


Return_code fill_void_lower (Picture32* result, int* line, Point offset) {

    if (!result) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!line)   { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    while (*line < offset.y) {

        load_line_default (result);
        *line += 1;
    }


    return SUCCESS;
}


Return_code copy_top_lower (Picture32* result, int* line, Picture32* top, Point offset) {

    if (!result) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!line)   { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!top)    { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    while (*line < offset.y + top->height) {

        copy_top_line (result, top, offset);
        *line += 1;
    }


    return SUCCESS;
}


#define TOP_START    (offset.x)
#define TOP_END      (offset.x + top->width)
#define BOTTOM_START (0)
#define BOTTOM_END   (bottom->width)

#define COMMON_START (my_min (offset.x, 0))
#define COMMON_END   (result->width - my_min (offset.x, 0))

// fills result line from top line and default remainder
// moves buffer pointers respectively

Return_code copy_top_line (Picture32* result, Picture32* top, Point offset) {

    if (!result) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!top)    { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    int current_column = COMMON_START;


    load_picture32_default (result, &current_column, TOP_START);


    write_picture32_to_picture32 (result, top, &current_column, TOP_END);


    load_picture32_default (result, &current_column, COMMON_END);


    return SUCCESS;
}


// fills result line from bottom line and default remainder
// moves buffer pointers respectively

Return_code copy_bottom_line (Picture32* result, Picture24* bottom, Point offset) {

    if (!result) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!bottom) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    int current_column = my_min (offset.x, 0);;


    load_picture32_default (result, &current_column, BOTTOM_START);


    write_picture24_to_picture32 (result, bottom, &current_column, BOTTOM_END);


    load_picture32_default (result, &current_column, COMMON_END);


    return SUCCESS;
}


Return_code load_line_default (Picture32* result) {

    if (!result) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    int current_column = 0;


    load_picture32_default (result, &current_column, result->width);


    return SUCCESS;
}


Return_code merge_line (Picture32* result, Picture32* top, Picture24* bottom, Point offset) {

    if (!result) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!top)    { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!bottom) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    int current_column = COMMON_START;

    write_picture32_to_picture32 (result, top,    &current_column, my_min (BOTTOM_START, TOP_END)); // first copy top
    write_picture24_to_picture32 (result, bottom, &current_column, my_min (TOP_START, BOTTOM_END)); // first copy bottom


    //--------------------------------------------------

    bool merge_flag = true;
    int merge_dist1 = offset.x + top->width - 0; // конец1 - начало2
    int merge_dist2 = bottom->width         - offset.x;
    int merge_count = my_min (merge_dist1, merge_dist2); // минимальное перекрытие, меньше нуля, если есть разрыв
    if (merge_count < 0) merge_flag = false; // в целом, не нужная строка


    if (merge_flag) merge_pictures_pixels  (result, top, bottom, &current_column, my_min (TOP_END, BOTTOM_END));
    else            load_picture32_default (result,              &current_column, my_min (TOP_END, BOTTOM_END));

    //--------------------------------------------------


    write_picture32_to_picture32 (result, top,    &current_column, TOP_END);    // second copy top
    write_picture24_to_picture32 (result, bottom, &current_column, BOTTOM_END); // second copy bottom


    return SUCCESS;
}


Return_code load_pixel32_from_pixel24 (Pixel_color32* dst, Pixel_color24* src) {

    if (!dst) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!src) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    dst->red   = src->red;
    dst->green = src->green;
    dst->blue  = src->blue;

    dst->transparency = BACKGROUND_TRANSPARENCY;


    return SUCCESS;
}


Return_code write_picture24_to_picture32 (Picture32* dst, Picture24* src, int* column, int max_column) {

    if (!dst)    { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!src)    { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!column) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    for (; *column < max_column; (*column)++) {

        load_pixel32_from_pixel24 (dst->buffer, src->buffer);


        dst->buffer += 1;
        src->buffer += 1;
    }


    return SUCCESS;
}


Return_code load_pixel32_from_pixel32 (Pixel_color32* dst, Pixel_color32* src) {

    if (!dst) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!src) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    dst->red   = src->red;
    dst->green = src->green;
    dst->blue  = src->blue;

    dst->transparency = src->transparency;


    return SUCCESS;
}


Return_code write_picture32_to_picture32 (Picture32* dst, Picture32* src, int* column, int max_column) {

    if (!dst)    { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!src)    { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!column) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    for (; *column < max_column; (*column)++) {

        load_pixel32_from_pixel32 (dst->buffer, src->buffer);


        dst->buffer += 1;
        src->buffer += 1;
    }


    return SUCCESS;
}


Return_code load_pixel32_default (Pixel_color32* dst) {

    if (!dst) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    dst->red   = DEFAULT_RED;
    dst->green = DEFAULT_GREEN;
    dst->blue  = DEFAULT_BLUE;

    dst->transparency = DEFAULT_TRANSPARENCY;


    return SUCCESS;
}


Return_code load_picture32_default (Picture32* dst, int* column, int max_column) {

    if (!dst)    { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!column) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    for (; *column < max_column; (*column)++) {

        load_pixel32_default (dst->buffer);


        dst->buffer += 1;
    }


    return SUCCESS;
}


Return_code merge_pixel (Pixel_color32* result, Pixel_color32* top, Pixel_color24* bottom) {

    if (!result) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!top)    { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!bottom) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    double coefficient = (double) top->transparency / 255;

    result->red   = (unsigned char) ((coefficient) * top->red   + (1 - coefficient) * bottom->red);
    result->green = (unsigned char) ((coefficient) * top->green + (1 - coefficient) * bottom->green);
    result->blue  = (unsigned char) ((coefficient) * top->blue  + (1 - coefficient) * bottom->blue);

    result->transparency = MERGED_TRANSPARENCY;


    return SUCCESS;
}


Return_code merge_pictures_pixels (Picture32* dst, Picture32* top, Picture24* bottom, int* column, int max_column) {

    if (!dst)    { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!top)    { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!bottom) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!column) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    for (; *column < max_column; (*column)++) {

        merge_pixel (dst->buffer, top->buffer, bottom->buffer);


        dst->buffer    += 1;
        top->buffer    += 1;
        bottom->buffer += 1;
    }


    return SUCCESS;
}


Buffer_saver3* save_buffers (Picture32* top, Picture24* bottom, Picture32* result) {

    if (!top)    { LOG_ERROR (BAD_ARGS); return nullptr; }
    if (!bottom) { LOG_ERROR (BAD_ARGS); return nullptr; }
    if (!result) { LOG_ERROR (BAD_ARGS); return nullptr; }


    Buffer_saver3* ans = (Buffer_saver3*) calloc (1, BUFFER_SAVER3_SIZE);


    ans->top    = top->buffer;
    ans->bottom = bottom->buffer;
    ans->result = result->buffer;


    return ans;
}


Return_code restore_buffers (Picture32* top, Picture24* bottom, Picture32* result, Buffer_saver3* saver) {

    if (!top)    { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!bottom) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!result) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!saver)  { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    top->buffer    = saver->top;
    bottom->buffer = saver->bottom;
    result->buffer = saver->result;


    free (saver);


    return SUCCESS;
}

