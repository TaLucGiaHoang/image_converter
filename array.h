#ifndef __ARRAY_H__
#define __ARRAY_H__

#include <stdlib.h>
#include <stdint.h>

#define DEBUG_PRINT (0)

void print_array(const uint8_t * buffer, int size);
int saveBufferToFile(const uint8_t* buffer, size_t size, const char* filename);

#endif /* __ARRAY_H__ */