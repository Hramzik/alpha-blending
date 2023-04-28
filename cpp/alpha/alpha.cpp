

//--------------------------------------------------


#include "../../hpp/alpha/alpha.hpp"


//--------------------------------------------------


#include "pixels.cpp"
#include "lines.cpp"
#include "line_phases.cpp"


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

