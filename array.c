#include <stdio.h>
#include <string.h>
#include "array.h"

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