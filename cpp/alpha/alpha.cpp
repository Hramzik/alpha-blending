

//--------------------------------------------------


#include "../../hpp/alpha/alpha.hpp"


//--------------------------------------------------


#include "pic_parts.cpp"

//---------------------------

#ifdef OPTIMIZE_ALPHA_BLENDING // подстановка оптимизированных версий
    #define load_buffer32_from_buffer24 load_buffer32_from_buffer24_opt
    #define load_buffer32_from_buffer32 load_buffer32_from_buffer32_opt
    #define load_buffer32_default       load_buffer32_default_opt
    #define merge_buffer32              merge_buffer32_opt
#endif

#include "lines.cpp"

#undef load_buffer32_from_buffer24
#undef load_buffer32_from_buffer32
#undef load_buffer32_default
#undef merge_buffer32

//---------------------------

#include "line_parts.cpp"
#include "pixels.cpp"

#ifdef OPTIMIZE_ALPHA_BLENDING
    #include "line_parts_opt.cpp"
    #include "pixels_opt.cpp"
#endif


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


//-------------- buffer saver code -----------------


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

