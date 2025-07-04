// $ gcc main.c logo_image_lvds.c -o logo_convert_color
// $ ./logo_convert_color

#include <stdio.h>
#include <string.h>
#include <stdint.h>


extern const uint8_t __logo_img_dat_lvds[] __attribute__ ((aligned (256))); // 1280*800*4UL
uint32_t __logo_img_dat_lvds_size = 1280*800*4UL;

const uint8_t input[] __attribute__ ((aligned (256))) = {
    0xf1,0xf2,0xf3,0xf4,
    0xf1,0xf2,0xf3,0xf4,
    0xf1,0xf2,0xf3,0xf4,
    0xf1,0xf2,0xf3,0xf4,
    0xf1,0xf2,0xf3,0xf4,
    0xf1,0xf2,0xf3,0xf4,
    0xf1,0xf2,0xf3,0xf4,
    0xf1,0xf2,0xf3,0xf4,
};

uint8_t output[1280*900*4] __attribute__ ((aligned (256))); // 1280*800*4UL

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

int main(int argc, char** argv)
{

    const char *dst = "output_rgb888.txt";
    const uint8_t *p_input_image = __logo_img_dat_lvds;
    uint8_t *p_output_image = output;
    size_t input_size = __logo_img_dat_lvds_size;  //sizeof(input);
    size_t output_size = 0; //sizeof(output);
    char file_name[100];
    
    if(argc >= 2)
    {
        printf("Input file: %s\n", argv[1]);
        strcpy(file_name, argv[1]);
    }


    printf("Input Image (%ld bytes)\n", input_size);
    print_array(p_input_image, input_size);
    
    /* Convert data */
    int i = 0, j = 0;
    for(; i < input_size; i+=4)
    {
        p_output_image[j] = p_input_image[i];
        p_output_image[j+1] = p_input_image[i+1];
        p_output_image[j+2] = p_input_image[i+2];
        // p_input_image[j+3]; // skipped data
        j += 3;
    }
    output_size = j;
    printf("Output Image (%ld bytes)\n", output_size);
    print_array(p_output_image, output_size);

    FILE *f_in;
    if(!(f_in = fopen("argb8888.raw", "wb"))) // Save as binay file
    {
        printf("[%s:%d] Error: Cannot open output file\n", __FILE__, __LINE__);
		return (-1);
    }
    
    fwrite(p_input_image, 1 , input_size, f_in);
    fclose(f_in);
    printf("Save to argb8888.raw file\n");

    FILE *f_out;
    if(!(f_out = fopen("rgb888.raw", "wb"))) // Save as binay file
    {
        printf("[%s:%d] Error: Cannot open output file\n", __FILE__, __LINE__);
		return (-1);
    }
    fwrite(p_output_image, 1 , output_size, f_out);
    fclose(f_out);
    printf("Save to rgb888.raw file\n");

    

    return 0;
}