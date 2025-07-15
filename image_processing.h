#ifndef __IMAGE_PROCESSNG__
#define __IMAGE_PROCESSNG__

#include <stdlib.h>
#include <stdint.h>

//  31       23       15       7        0
//  |________|________|________|________|
//   00000000 bbbbbb00 gggggg00 rrrrrr00            RGB666 32-bit
//            bbbbbb00 gggggg00 rrrrrr00            RGB666 24-bit
//                  bb bbbbgggg ggrrrrrr            RGB666 18-bit
#define RGB666_18BIT    0
#define RGB666_24BIT    1
#define RGB666_32BIT    2


size_t cvt_argb8888_rgb888(const uint8_t * input_image, size_t input_size, uint8_t *output_image, uint8_t is_32bit_pixel);
size_t process_rgb666_18bit(const uint8_t * input_image, size_t input_size, uint8_t *output_image);
size_t cvt_argb8888_rgb666(const uint8_t * input_image, size_t input_size, uint8_t *output_image, uint8_t select_type);
size_t generate_argb_image(uint8_t * buffer, size_t size, uint32_t color);

#endif /* __IMAGE_PROCESSNG__ */