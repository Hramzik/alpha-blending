#ifndef ALPHA_HPP_INCLUDED
#define ALPHA_HPP_INCLUDED
//--------------------------------------------------


#include "../common/common.hpp"


//--------------------------------------------------


#include "structs.hpp"


//--------------------------------------------------


#include "constants.hpp"


//--------------------------------------------------


Return_code merge_pictures              (Picture32* result, Picture32* top, Picture24* bottom, Point offset);
Return_code copy_pixels_until_equal_top (Picture32* result, Picture24* bottom, Point offset);
Return_code merge_pixels_from_equal_top (Picture32* result, Picture32* top, Picture24* bottom, Point offset);

Return_code copy_line  (Picture32* result, Picture24* bottom);
Return_code merge_line (Picture32* result, Picture32* top, Picture24* bottom, Point offset);

Return_code load_pixel32_from_pixel24 (Pixel_color32* dst, Pixel_color24* src);
Return_code load_pixel32_default      (Pixel_color32* dst);
Return_code merge_pixels              (Pixel_color32* result, Pixel_color32* top, Pixel_color24* bottom);

Buffer_saver3* save_buffers    (Picture32* top, Picture24* bottom, Picture32* result);
Return_code    restore_buffers (Picture32* top, Picture24* bottom, Picture32* result, Buffer_saver3* saver);


//--------------------------------------------------
#endif