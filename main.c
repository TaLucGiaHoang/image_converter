// $ gcc main.c array.c image_processing.c -o image_converter
// $ ./image_converter

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>  // malloc()
#include "array.h"
#include "image_processing.h"

#define SELECT_TEST_CASE (2) // 0: constant image, 1: small array, 2: mono-color image (Red/Green/Blue)

#if (SELECT_TEST_CASE == 0)
extern const uint8_t __logo_img_dat_lvds[] __attribute__ ((aligned (256))); // 1280*800*4UL
uint32_t __logo_img_dat_lvds_size = 1280*800*4UL;
#endif

uint8_t sample_array[] __attribute__ ((aligned (256))) = {
    0xFF,0xFF,0xFF,0x00,
    0xFF,0xFF,0xFF,0x00,
    0xFF,0xFF,0xFF,0x00,
    0xFF,0xFF,0xFF,0x00,
    0x00,0x00,0x00,0x00,
    0xFF,0x00,0x00,0x00,    // red
    0x00,0xFF,0x00,0x00,    // green
    0x00,0x00,0xFF,0x00,    // blue

    // 0x1B,0x2C,0x3D,0x4E,
    // 0x5B,0x6C,0x7D,0x8E,
    // 0x9B,0xAC,0xBD,0xCE,
    // 0xA3,0xB3,0xC3,0xF3,
    // 0xA4,0xB4,0xC4,0xF4,
    // 0xA5,0xB5,0xC5,0xF5,
    // 0xA6,0xB6,0xC6,0xF6,
    // 0xA7,0xB7,0xC7,0xF7,
};

uint8_t output_buffer[1280*900*4] __attribute__ ((aligned (256))); // 1280*800*4UL = 4608000


int main(int argc, char** argv)
{
    uint8_t *p_input_image;
    uint8_t *p_output_image;
    size_t input_size;
    size_t output_size;
    size_t write_size = 0;
    char file_name[100];
    
    /* Select input images */
    if(argc >= 2)
    {
        printf("Input file: %s\n", argv[1]);
        strcpy(file_name, argv[1]);
    }
    else
    {
#if (SELECT_TEST_CASE == 0)
    p_input_image = __logo_img_dat_lvds;
    p_output_image = output_buffer;
    input_size = __logo_img_dat_lvds_size;  //sizeof(input);
    output_size = 4608000;  // 1280*800*4UL = 4608000
#elif (SELECT_TEST_CASE == 1)
    p_input_image = sample_array;
    input_size = sizeof(sample_array);
    p_output_image = output_buffer;
    output_size = input_size;
#else // (SELECT_TEST_CASE == 2)
    p_input_image = (uint8_t*)malloc(4608000);
    p_output_image = output_buffer;
    input_size = 4608000;   // 1280*800*4UL = 4608000
    output_size = 4608000;  // 1280*800*4UL = 4608000

    generate_argb_image(p_input_image, input_size, 0xFFFFFFFF); // 0xFFFFFFFF: white
#endif
    }

    printf("Input Image (%ld bytes)\n", input_size);
    saveBufferToFile(p_input_image, input_size, "white_argb8888_32bit_1280x800.raw");
#if (DEBUG_PRINT == 1)
    printf("Input data ARGB8888:\n");
    print_array(p_input_image, input_size);
#endif

    /* Clean output buffer */
    memset(p_output_image, 0, output_size);

    /* Convert ARGB8888 to RGB888 (24bit/pixel) */
    write_size = cvt_argb8888_rgb888(p_input_image, input_size, p_output_image, 0);
    saveBufferToFile(p_output_image, write_size, "white_rgb888_24bit_1280x800.raw");
#if (DEBUG_PRINT == 1)
    printf("Convert ARGB8888 to RGB888 (24bit/pixel) output:\n");
    print_array(p_output_image, write_size);
#endif

    /* Clean output buffer */
    memset(p_output_image, 0, output_size);

    /* Convert ARGB8888 to RGB888 (32bit/pixel) */
    write_size = cvt_argb8888_rgb888(p_input_image, input_size, p_output_image, 1);
    saveBufferToFile(p_output_image, write_size, "white_rgb888_32bit_1280x800.raw");
#if (DEBUG_PRINT == 1)
    printf("Convert ARGB8888 to RGB888 (32bit/pixel) output:\n");
    print_array(p_output_image, write_size);
#endif

    /* Clean output buffer */
    memset(p_output_image, 0, output_size);

    /* Convert ARGB8888 to RGB666 (24bit/pixel) */
    write_size = cvt_argb8888_rgb666(p_input_image, input_size, p_output_image, RGB666_24BIT);
    saveBufferToFile(p_output_image, write_size, "white_rgb666_24bit_1280x800.raw");
#if (DEBUG_PRINT == 1)
    printf("Convert ARGB8888 to RGB666 (24bit/pixel) output:\n");
    print_array(p_output_image, write_size);
#endif

    /* Clean output buffer */
    memset(p_output_image, 0, output_size);

    /* Convert ARGB8888 to RGB666 (32bit/pixel) */
    write_size = cvt_argb8888_rgb666(p_input_image, input_size, p_output_image, RGB666_32BIT);
    saveBufferToFile(p_output_image, write_size, "white_rgb666_32bit_1280x800.raw");
#if (DEBUG_PRINT == 1)
    printf("Convert ARGB8888 to RGB666 (32bit/pixel) output:\n");
    print_array(p_output_image, write_size);
#endif

    /* Clean output buffer */
    memset(p_output_image, 0, output_size);

    /* Convert ARGB8888 to RGB666 (18bit/pixel) */
    write_size = cvt_argb8888_rgb666(p_input_image, input_size, p_output_image, RGB666_18BIT);
    saveBufferToFile(p_output_image, write_size, "white_rgb666_18bit_1280x800.raw");
#if (DEBUG_PRINT == 1)
    printf("Convert ARGB8888 to RGB666 (18bit/pixel) output:\n");
    print_array(p_output_image, write_size);
#endif
    return 0;
}