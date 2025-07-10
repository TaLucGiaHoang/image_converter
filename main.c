// $ gcc main.c logo_image_lvds.c -o logo_convert_color
// $ ./logo_convert_color

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define SMALL_ARRAY_TEST (1)
#define DEBUG_PRINT (1)

//  31       23       15       7        0
//  |________|________|________|________|
//            bbbbbb00 gggggg00 rrrrrr00            RGB666_24BIT
//   00000000 bbbbbb00 gggggg00 rrrrrr00            RGB666_32BIT
//                  bb bbbbgggg ggrrrrrr            RGB666_18BIT
#define RGB666_18BIT    0
#define RGB666_24BIT    1
#define RGB666_32BIT    2



#if (SMALL_ARRAY_TEST == 0)
extern const uint8_t __logo_img_dat_lvds[] __attribute__ ((aligned (256))); // 1280*800*4UL
uint32_t __logo_img_dat_lvds_size = 1280*800*4UL;
#endif

const uint8_t sample_array[] __attribute__ ((aligned (256))) = {
    0xFF,0xFF,0xFF,0x00,
    0xFF,0xFF,0xFF,0x00,
    0xFF,0xFF,0xFF,0x00,
    0xFF,0xFF,0xFF,0x00,
    0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,

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
uint8_t output_buffer2[1280*900*4] __attribute__ ((aligned (256))); // 1280*800*4UL = 4608000

void print_array(const uint8_t * buffer, int size)
{
    const int group = 16;
    int _size = size > 100 ? 100 : size; // set maximum print is 100 elements
    for(int i = 0; i < _size; i++)
    {
        if( (i % group == 0) && (i > 0) )
        {
            printf("\n");
        }
        printf("0x%.2x ", buffer[i]);
    }
    printf("\n");
}

size_t cvt_argb8888_rgb888(const uint8_t * input_image, size_t input_size, uint8_t *output_image, uint8_t is_32bit_pixel)
{
    size_t output_size = 0;
    int i = 0, j = 0;

    if((!input_image) || (!output_image)) {
        printf("[%s] NULL parameters.\n", __func__);
    }
    /* Convert data */
    for(; i < input_size; i = i + 4)
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
    printf("(type == RGB666_18BIT)\n");
    if(input_size % 4)
    {
        printf("Error: Input size %ld bytes is not suitable\n", input_size);
    }
    
    while(1)
    {
        /* Covert a block of 3 bytes of colors in ARGB8888 ignoring A plane to block of 3 bytes of colors in RGB666 */
        uint8_t * block_rgb666_3bytes = output_image; // 4 pixels of RGB666 image = 4 * 18 = 72 bits = 9 bytes
        uint8_t block_rgb888_4bytes[4];
        
        memcpy((void*)block_rgb888_4bytes, (void*)&input_image[i], 4);

printf("before: ");
        print_array(block_rgb888_4bytes, 4);
        /* eliminate first 2 bits */
        block_rgb888_4bytes[0] >>= 2;       // 0 0 b0 b0 b0 b0 b0 b0
        block_rgb888_4bytes[1] >>= 2;       // 0 0 g0 g0 g0 g0|g0 g0
        block_rgb888_4bytes[2] >>= 2;       // 0 0 r0 r0|r0 r0 r0 r0
        block_rgb888_4bytes[3] >>= 2;       // 0 0 b1 b1 b1 b1 b1 b1
printf("after : ");
        print_array(block_rgb888_4bytes, 4);
        
        block_rgb666_3bytes[j]   = (uint8_t)(block_rgb888_4bytes[1] << 6) | (uint8_t)(block_rgb888_4bytes[0]);         // g0 g0 b0 b0 b0 b0 b0 b0  G0B0
        block_rgb666_3bytes[j+1] = (uint8_t)(block_rgb888_4bytes[2] << 4) | (uint8_t)(block_rgb888_4bytes[1] >> 2);  // r0 r0 r0 r0 g0 g0 g0 g0  R0G0
        block_rgb666_3bytes[j+2] = (uint8_t)(block_rgb888_4bytes[3] << 2) | (uint8_t)(block_rgb888_4bytes[2] >> 4);  // b1 b1 b1 b1 b1 b1 r0 r0  B1R0 
        // ignore alpha color byte block_rgb888_4bytes[i+3];

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
    // if(select_type == RGB666_24BIT || select_type == RGB666_32BIT)
    {
        for(; i < input_size; i = i + 4)
        {
            output_image[j++] = input_image[i]   & 0xFC;
            output_image[j++] = input_image[i+1] & 0xFC;
            output_image[j++] = input_image[i+2] & 0xFC;
            
            if(select_type == RGB666_32BIT)
            {
                output_image[j++] = 0x00; // any value
            }
        }
    }

    if(select_type == RGB666_18BIT)
    {
        input_size = j;
        memcpy(output_buffer2, output_image, input_size);  // copy to temp buffer 2
        process_rgb666_18bit(output_buffer2, input_size, output_image);

    }
    output_size = j;
    return output_size;
}

int saveBufferToFile(const uint8_t* buffer, size_t size, const char* filename)
{
    FILE *fp;
    char saved_name[100] = "";
    // const char *extension = ".bin";
    size_t n = 0;
    memset(saved_name, 0, 100);
    
    strcat(saved_name, filename);
    // strcat(saved_name, extension);
    
    if(!(fp = fopen(saved_name, "wb"))) // Save as binay file
    {
        printf("[%s:%d] Error: Cannot open output file\n", __FILE__, __LINE__);
        return (-1);
    }
    n = fwrite(buffer, 1 , size, fp);
    fclose(fp);
    printf("Save to %s file (%ld bytes)\n", saved_name, size);
    return 0;
}

int main(int argc, char** argv)
{
    const uint8_t *p_input_image;
    uint8_t *p_output_image;
    size_t input_size;
    size_t output_size;
    size_t write_size = 0;
    char file_name[100];
    
    
#if (SMALL_ARRAY_TEST == 0)
    p_input_image = __logo_img_dat_lvds;
    p_output_image = output_buffer;
    input_size = __logo_img_dat_lvds_size;  //sizeof(input);
    output_size = 4608000;  // 1280*800*4UL = 4608000
#else // (SMALL_ARRAY_TEST == 1)
    p_input_image = sample_array;
    input_size = sizeof(sample_array);
    p_output_image = output_buffer;
    output_size = input_size;
#endif

    if(argc >= 2)
    {
        printf("Input file: %s\n", argv[1]);
        strcpy(file_name, argv[1]);
    }


    printf("Input Image (%ld bytes)\n", input_size);
#if (DEBUG_PRINT == 1)
    printf("ARGB8888 input:\n");
    print_array(p_input_image, input_size);
#endif
    // saveBufferToFile(p_input_image, input_size, "argb8888.raw");
    
    /* Clean output buffer */
    memset(p_output_image, 0, output_size);

    /* Convert ARGB8888 to RGB888 (24bit/pixel) */
    write_size = cvt_argb8888_rgb888(p_input_image, input_size, p_output_image, 0);
    // saveBufferToFile(p_output_image, write_size, "rgb888_24bit.raw");
#if (DEBUG_PRINT == 1)
    printf("Convert ARGB8888 to RGB888 (24bit/pixel) output:\n");
    print_array(p_output_image, write_size);
#endif

    // memset(p_output_image, 0, output_size);
    
    // /* Convert ARGB8888 to RGB888 (32bit/pixel) */
    // write_size = cvt_argb8888_rgb888(p_input_image, input_size, p_output_image, 1);
    // saveBufferToFile(p_output_image, write_size, "rgb888_32bit.raw");
// #if (DEBUG_PRINT == 1)
    // printf("Convert ARGB8888 to RGB888 (32bit/pixel) output:\n");
    // print_array(p_output_image, write_size);
// #endif

    memset(p_output_image, 0, output_size);
    
    /* Convert ARGB8888 to RGB666 (24bit/pixel) */
    write_size = cvt_argb8888_rgb666(p_input_image, input_size, p_output_image, RGB666_24BIT);
    // saveBufferToFile(p_output_image, write_size, "rgb666_32bit.raw");
#if (DEBUG_PRINT == 1)
    printf("Convert ARGB8888 to RGB666 (24bit/pixel) output:\n");
    print_array(p_output_image, write_size);
#endif

    // memset(p_output_image, 0, output_size);
    
    // /* Convert ARGB8888 to RGB666 (32bit/pixel) */
    // write_size = cvt_argb8888_rgb666(p_input_image, input_size, p_output_image, RGB666_32BIT);
    // saveBufferToFile(p_output_image, write_size, "rgb666_32bit.raw");
// #if (DEBUG_PRINT == 1)
    // printf("Convert ARGB8888 to RGB666 (32bit/pixel) output:\n");
    // print_array(p_output_image, write_size);
// #endif

    memset(p_output_image, 0, output_size);
    
    /* Convert ARGB8888 to RGB666 (18bit/pixel) */
    write_size = cvt_argb8888_rgb666(p_input_image, input_size, p_output_image, RGB666_18BIT);
    // saveBufferToFile(p_output_image, write_size, "rgb666_18bit.raw");
#if (DEBUG_PRINT == 1)
    printf("Convert ARGB8888 to RGB666 (18bit/pixel) output:\n");
    print_array(p_output_image, write_size);
#endif
    return 0;
}