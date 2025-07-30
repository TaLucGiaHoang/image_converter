// $ gcc main.c logo_image_lvds.c -o logo_convert_color
// $ ./logo_convert_color

#include <stdio.h>
#include <string.h>
#include "array.h"
#include "image_processing.h"


size_t cvt_argb8888_rgb888(const uint8_t * input_image, size_t input_size, uint8_t *output_image, uint8_t is_32bit_pixel)
{
    size_t output_size = 0;
    int i = 0, j = 0;

    if((!input_image) || (!output_image)) {
        printf("[%s] NULL parameters.\n", __func__);
    }
    /* Convert data */
    for(; i < input_size; i+=4)
    {
        output_image[j++] = input_image[i];
        output_image[j++] = input_image[i+1];
        output_image[j++] = input_image[i+2];
        
        if(is_32bit_pixel)
        {
            output_image[j++] = 0x00; // any value
        }
    }
    output_size = j;
    return output_size;
}

size_t process_rgb666_18bit(const uint8_t * input_image, size_t input_size, uint8_t *output_image)
{
    size_t output_size = 0;
    int i = 0, j = 0;

    if(input_size % 4)
    {
        printf("Warning: Input size %ld bytes is not suitable\n", input_size);
    }
    
    while(1)
    {
        /* Covert a block of 3 bytes of colors in ARGB8888 ignoring A plane to block of 3 bytes of colors in RGB666 */
        uint8_t * p_block_3bytes_rgb666_18bit = output_image; // 4 pixels of RGB666 image = 4 * 18 = 72 bits = 9 bytes
        uint8_t block_4bytes_rgb666_24bit[4];

        memcpy((void*)block_4bytes_rgb666_24bit, (void*)&input_image[i], 4);

#if (DEBUG_PRINT == 1)
        // printf("before: ");  
        // print_array(block_4bytes_rgb666_24bit, 4);
#endif
        // RGB666 24-bit (4 bytes)
        // |7  6  5  4  3  2  1 0|
        // |b0 b0 b0 b0 b0 b0 0 0|
        // |g0 g0 g0 g0 g0 g0 0 0|
        // |r0 r0 r0 r0 r0 r0 0 0|
        // |b1 b1 b1 b1 b1 b1 0 0|
        //
        // Convert to RGB666 18-bit (3 bytes)
        // |7  6  5  4  3  2  1  0 |
        // |g0 g0.b0 b0 b0 b0 b0 b0|
        // |r0 r0 r0 r0.g0 g0 g0 g0|
        // |b1 b1 b1 b1 b1 b1.r0 r0|
        //
        p_block_3bytes_rgb666_18bit[j]   = (uint8_t)(block_4bytes_rgb666_24bit[1] << 4) | (uint8_t)(block_4bytes_rgb666_24bit[0] >> 2);  // g0 g0 b0 b0 b0 b0 b0 b0  G0B0
        p_block_3bytes_rgb666_18bit[j+1] = (uint8_t)(block_4bytes_rgb666_24bit[2] << 2) | (uint8_t)(block_4bytes_rgb666_24bit[1] >> 4);  // r0 r0 r0 r0 g0 g0 g0 g0  R0G0
        p_block_3bytes_rgb666_18bit[j+2] = (uint8_t)(block_4bytes_rgb666_24bit[3] << 0) | (uint8_t)(block_4bytes_rgb666_24bit[2] >> 6);  // b1 b1 b1 b1 b1 b1 r0 r0  B1R0
#if (DEBUG_PRINT == 1)
        // printf("after : ");
        // print_array(&p_block_3bytes_rgb666_18bit[j], 3);
#endif
        j += 3;
        i += 4;
        if(i >= input_size)
        {
            break;
        }
    }

    output_size = j;
    return output_size;
}

size_t cvt_argb8888_rgb666(const uint8_t * input_image, size_t input_size, uint8_t *output_image, uint8_t select_type)
{
    size_t output_size = 0;
    int i = 0, j = 0;

    if((!input_image) || (!output_image)) {
        printf("[%s] NULL parameters.\n", __func__);
    }
    /* Convert data */
    for(; i < input_size; i+=4)
    {
        output_image[j++] = input_image[i]   & 0xFC;
        output_image[j++] = input_image[i+1] & 0xFC;
        output_image[j++] = input_image[i+2] & 0xFC;

        if(select_type == RGB666_32BIT)
        {
            output_image[j++] = 0x00; // any value
        }
    }
    output_size = j;

    if(select_type == RGB666_18BIT)
    {
        input_size = j;
        output_size = process_rgb666_18bit(output_image, input_size, output_image);
    }

    return output_size;
}

size_t generate_argb_image(uint8_t * buffer, size_t size, uint32_t color)
{   
    uint8_t * p_color = (uint8_t *)&color;
    if(!buffer)
    {
        printf("[%s] NULL parameters.\n", __func__);
        return -1;
    }

    if(size % 4)
    {
        printf("Warning: size=%ld bytes is not a multiple of 4.\n", size);
    }
//#if (DEBUG_PRINT == 1)
    printf("[%s] Create an array with color: 0x%.08x, ", __func__, color);
    print_array(p_color, 4);
//#endif
    for(int i = 0; i < size; i+=4)
    {
        buffer[i]   = p_color[0];
        buffer[i+1] = p_color[1];
        buffer[i+2] = p_color[2];
        buffer[i+3] = p_color[3];
    }
    return 0;
}
