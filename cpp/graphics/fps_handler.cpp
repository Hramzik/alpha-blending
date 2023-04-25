

//--------------------------------------------------
#include "../../hpp/graphics/graphics.hpp"

#include "../../lib/logs.cpp"
//--------------------------------------------------


Fps_handler* fps_handler_ctor (void) {

    Fps_handler* result = (Fps_handler*) calloc (1, FPS_HANDLER_SIZE);


    result->cur_frame_time = clock ();
    result->old_frame_time = 0;


    result->frame_number = 0;


    result->sum_delays_ms = 0;
    result->sum_fps       = 0;


    return result;
}


Return_code fps_handler_dtor (Fps_handler* handler) {

    if (!handler) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    free (handler);


    return SUCCESS;
}


Return_code fps_handler_next_frame (Fps_handler* handler) {

    if (!handler) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    handler->old_frame_time = handler->cur_frame_time;
    handler->cur_frame_time = clock ();


    handler->frame_number += 1;


    handler->sum_delays_ms += fps_handler_get_delay_ms (handler);
    handler->sum_fps       += fps_handler_get_fps      (handler);


    return SUCCESS;
}


double fps_handler_get_delay_ms (Fps_handler* handler) {

    if (!handler) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    return (1000 * ( (double) handler->cur_frame_time - (double) handler->old_frame_time) / CLOCKS_PER_SEC);
}


double fps_handler_get_fps (Fps_handler* handler) {

    if (!handler) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    return 1000 / fps_handler_get_delay_ms (handler);
}


double fps_handler_get_avg_delay (Fps_handler* handler) {

    if (!handler) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    return handler->sum_delays_ms / (double) handler->frame_number;
}


double fps_handler_get_avg_fps (Fps_handler* handler) {

    if (!handler) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    return handler->sum_fps / (double) handler->frame_number;
}


Return_code fps_handler_print (Fps_handler* handler) {

    if (!handler) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    if (handler->frame_number == 1 || handler->frame_number == 10 || handler->frame_number == 1000) {

        printf ("cur fps = %10lf, cur delay = %lf\n",
            fps_handler_get_fps      (handler),
            fps_handler_get_delay_ms (handler));

        printf ("frame number = %zd, avg fps = %10lf, avg delay = %lf\n",
            handler->frame_number,
            fps_handler_get_avg_fps   (handler),
            fps_handler_get_avg_delay (handler));
    }


    return SUCCESS;
}

