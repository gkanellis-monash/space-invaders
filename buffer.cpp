#include "buffer.h"


uint32_t rgb_to_uint32(uint8_t r, uint8_t g, uint8_t b) {
    return (r << 24) | (g << 16) | (b << 8) | 255;
}



void buffer_clear(Buffer* buffer, uint32_t color) {
    for(size_t i = 0; i < buffer->width * buffer->height; ++i) {
        buffer->data[i] = color;
    }
}
