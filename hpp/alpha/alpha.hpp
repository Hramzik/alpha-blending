#ifndef ALPHA_HPP_INCLUDED
#define ALPHA_HPP_INCLUDED
//--------------------------------------------------


#include "../common/common.hpp"


//--------------------------------------------------


#include "../../lib/logs.hpp"


//--------------------------------------------------


#include "structs.hpp"


//--------------------------------------------------


#include "constants.hpp"


//--------------------------------------------------


Return_code merge_pictures              (Picture32* result, Picture32* top, Picture24* bottom, Point offset);

Return_code copy_top_upper (Picture32* result, int* line, Picture32* top, Point offset);
Return_code fill_void_upper (Picture32* result, int* line);
Return_code copy_bottom_upper (Picture32* result, int* line, Picture32* top, Picture24* bottom, Point offset);
Return_code merge_top_with_bottom (Picture32* result, int* line, Picture32* top, Picture24* bottom, Point offset);
Return_code copy_bottom_lower (Picture32* result, int* line, Picture24* bottom, Point offset);
Return_code fill_void_lower (Picture32* result, int* line, Point offset);
Return_code copy_top_lower (Picture32* result, int* line, Picture32* top, Point offset);

Return_code copy_top_line (Picture32* result, Picture32* top, Point offset);
Return_code copy_bottom_line (Picture32* result, Picture24* bottom, Point offset);
Return_code load_line_default (Picture32* result);
Return_code merge_line (Picture32* result, Picture32* top, Picture24* bottom, Point offset);


Return_code load_pixel32_from_pixel24 (Pixel_Color32* dst, Pixel_Color24* src);
Return_code load_buffer32_from_buffer24 (Picture32* dst, Picture24* src, int* column, int max_column);

Return_code load_pixel32_from_pixel32 (Pixel_Color32* dst, Pixel_Color32* src);
Return_code load_buffer32_from_buffer32 (Picture32* dst, Picture32* src, int* column, int max_column);

Return_code load_pixel32_default      (Pixel_Color32* dst);
Return_code load_buffer32_default (Picture32* dst, int* column, int max_column);

Return_code merge_pixel (Pixel_Color32* result, Pixel_Color32* top, Pixel_Color24* bottom);
Return_code merge_buffer32 (Picture32* dst, Picture32* top, Picture24* bottom, int* column, int max_column);


Buffer_saver3* save_buffers    (Picture32* top, Picture24* bottom, Picture32* result);
Return_code    restore_buffers (Picture32* top, Picture24* bottom, Picture32* result, Buffer_saver3* saver);


//--------------------------------------------------
#endif