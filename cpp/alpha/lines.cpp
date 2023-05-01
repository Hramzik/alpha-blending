

//--------------------------------------------------


#include "../../hpp/alpha/alpha.hpp"


//--------------------------------------------------


#define TOP_START    (offset.x)
#define TOP_END      (offset.x + top->width)
#define BOTTOM_START (0)
#define BOTTOM_END   (bottom->width)

#define COMMON_START (my_min (offset.x, 0))
#define COMMON_END   (COMMON_START + result->width)


//--------------------------------------------------


// fills result line from top line and default remainder
// moves buffer pointers respectively

Return_code copy_top_line (Picture32* result, Picture32* top, Point offset) {

    if (!result) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!top)    { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    int current_column = COMMON_START;


    load_buffer32_default (result, &current_column, TOP_START);


    load_buffer32_from_buffer32 (result, top, &current_column, TOP_END);


    load_buffer32_default (result, &current_column, COMMON_END);


    return SUCCESS;
}


// fills result line from bottom line and default remainder
// moves buffer pointers respectively

Return_code copy_bottom_line (Picture32* result, Picture24* bottom, Point offset) {

    if (!result) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!bottom) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    int current_column = COMMON_START;


    load_buffer32_default (result, &current_column, BOTTOM_START);


    load_buffer32_from_buffer24 (result, bottom, &current_column, BOTTOM_END);


    load_buffer32_default (result, &current_column, COMMON_END);


    return SUCCESS;
}


Return_code load_line_default (Picture32* result) {

    if (!result) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    int current_column = 0;


    load_buffer32_default (result, &current_column, result->width);


    return SUCCESS;
}


Return_code merge_line (Picture32* result, Picture32* top, Picture24* bottom, Point offset) {

    if (!result) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!top)    { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!bottom) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    int current_column = COMMON_START;

    load_buffer32_from_buffer32 (result, top,    &current_column, my_min (BOTTOM_START, TOP_END)); // first copy top
    load_buffer32_from_buffer24 (result, bottom, &current_column, my_min (TOP_START, BOTTOM_END)); // first copy bottom


    //--------------------------------------------------

    bool merge_flag = true;
    int merge_dist1 = offset.x + top->width - 0; // конец1 - начало2
    int merge_dist2 = bottom->width         - offset.x;
    int merge_count = my_min (merge_dist1, merge_dist2); // минимальное перекрытие, меньше нуля, если есть разрыв
    if (merge_count < 0) merge_flag = false;


    if (merge_flag) merge_buffer32        (result, top, bottom, &current_column, my_min (TOP_END,   BOTTOM_END));
    else            load_buffer32_default (result,              &current_column, my_max (TOP_START, BOTTOM_START));

    //--------------------------------------------------


    load_buffer32_from_buffer32 (result, top,    &current_column, TOP_END);    // second copy top
    load_buffer32_from_buffer24 (result, bottom, &current_column, BOTTOM_END); // second copy bottom


    return SUCCESS;
}


//--------------------------------------------------


#undef TOP_START
#undef TOP_END
#undef BOTTOM_START
#undef BOTTOM_END

#undef COMMON_START
#undef COMMON_END


//--------------------------------------------------

