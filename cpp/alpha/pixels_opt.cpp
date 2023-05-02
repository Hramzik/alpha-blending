

//--------------------------------------------------


#include "../../hpp/alpha/alpha.hpp"


//--------------------------------------------------


Return_code load_8_pixels32_from_pixels24 (Pixel_Color32* dst, Pixel_Color24* src) {

    if (!dst) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!src) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    __m256i src_offsets = _mm256_setr_epi32      ( 0, 3, 6, 9, 12, 15, 18, 21); // offsets of bottom pixels bytes
    __m256i src_pixels  = _mm256_i32gather_epi32 ( (int*) src, src_offsets, 1); // on alpha place - trash
    // вид: agbr (inversed)


    //--------- установка прозачности (opt) ------------

    __m256i alpha = _mm256_set1_epi8 (BACKGROUND_TRANSPARENCY);

    // в маске проверяются значения на индексах: 7, 15, 23, ..., 255
    // если там единица, в результат ставится char из прозрачности
    __m256i mask = _mm256_set1_epi32 (0x80'00'00'00); // переворачивается, так как __m256i хранит инты
                                                      // так же, как они лежат в памяти, то есть наоборот

    // прозрачность берем из переменной alpha
    __m256i result = _mm256_blendv_epi8 (src_pixels, alpha, mask);

    //--------------------------------------------------


    _mm256_storeu_si256 ( (__m256i*) dst, result);


    return SUCCESS;
}


Return_code load_8_pixels32_from_pixels32 (Pixel_Color32* dst, Pixel_Color32* src) {

    if (!dst) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!src) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    __m256i src_pixels = _mm256_loadu_si256 ( (__m256i*) src);

    _mm256_storeu_si256 ( (__m256i*) dst, src_pixels);


    return SUCCESS;
}


Return_code load_8_pixels32_default (Pixel_Color32* dst) {

    if (!dst) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    int red_part   = DEFAULT_RED;  // значение temp перевернуто нами
    int green_part = DEFAULT_GREEN << 8;
    int blue_part  = DEFAULT_BLUE  << 16;
    int alpha_part = DEFAULT_TRANSPARENCY << 24;

    __m256i temp  = _mm256_set1_epi32 (red_part | green_part | blue_part | alpha_part);

    _mm256_storeu_si256 ( (__m256i*) dst, temp); // запись перевернута little endian


    return SUCCESS;
}


Return_code merge_8_pixels (Pixel_Color32* result, Pixel_Color32* top, Pixel_Color24* bottom) {

    if (!result) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!top)    { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!bottom) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    // color = (1-a) * bottom + a * top = a * (top - bottom) + bottom

    __m256i top_pixels     = _mm256_loadu_si256   ( (__m256i*) top);
    __m256i bottom_offsets = _mm256_setr_epi32      ( 0, 3, 6, 9, 12, 15, 18, 21);       // offsets of bottom pixels bytes
    __m256i bottom_pixels  = _mm256_i32gather_epi32 ( (int*) bottom, bottom_offsets, 1); // on alpha place - trash

    __m256i alpha_mask     = _mm256_set1_epi32 (0xFF'00'00'00);
            alpha_mask     = _mm256_and_si256  (top_pixels, alpha_mask);
    __m256i green_alpha    = _mm256_srli_epi32 (alpha_mask, 24);          // used for green pixels
            alpha_mask     = _mm256_srli_epi32 (alpha_mask, 8);           // (temp) used on the next line
    __m256i red_blue_alpha = _mm256_or_si256   (green_alpha, alpha_mask); // used for red and blue pixels

    __m256i green_mask   = _mm256_set1_epi32  (0x00'00'FF'00);
    __m256i green_res    = _mm256_and_si256   (top_pixels,    green_mask);
    __m256i green_bottom = _mm256_and_si256   (bottom_pixels, green_mask);
            green_res    = _mm256_sub_epi16   (green_res,     green_bottom);
            green_res    = _mm256_mulhi_epi16 (green_res,    green_alpha);
            green_res    = _mm256_slli_epi16  (green_res, 8); // битово сдвинуть в правильное место
            green_res    = _mm256_add_epi16   (green_res,    green_bottom);

    __m256i red_blue_mask   = _mm256_set1_epi32  (0x00'FF'00'FF);
    __m256i red_blue_res    = _mm256_and_si256   (top_pixels,    red_blue_mask);
    __m256i red_blue_bottom = _mm256_and_si256   (bottom_pixels, red_blue_mask);
            red_blue_res    = _mm256_sub_epi16   (red_blue_res,  red_blue_bottom);
            red_blue_res    = _mm256_mullo_epi16 (red_blue_res, red_blue_alpha);  // разделить на 256 тут
            red_blue_res    = _mm256_srli_epi16  (red_blue_res, 8);               // битово сдвинуть в правильное место
            red_blue_res    = _mm256_add_epi16   (red_blue_res, red_blue_bottom);


    red_blue_res = _mm256_or_si256 (green_res, red_blue_res);


    //--------- установка прозачности (opt) ------------

    __m256i new_alpha     = _mm256_set1_epi32 (MERGED_TRANSPARENCY << 24);
    __m256i no_alpha_mask = _mm256_set1_epi32 (0x00'FF'FF'FF);


    red_blue_res = _mm256_and_si256 (red_blue_res, no_alpha_mask);
    red_blue_res = _mm256_or_si256  (red_blue_res, new_alpha);

    //--------------------------------------------------


    _mm256_storeu_si256 ( (__m256i*) result, red_blue_res);


    return SUCCESS;
}

