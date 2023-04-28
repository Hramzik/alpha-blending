#ifndef GRAPHICS_DSL_ON
#define GRAPHICS_DSL_ON


#define TOP_PIC    (mixer->media.top_pic)
#define BOTTOM_PIC (mixer->media.bottom_pic)
#define RESULT_PIC (mixer->media.result_pic)


//--------------------------------------------------


#else
#undef GRAPHICS_DSL_ON


#undef TOP_PIC
#undef BOTTOM_PIC
#undef RESULT_PIC


//--------------------------------------------------
#endif