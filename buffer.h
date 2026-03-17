#pragma once

#include <cstddef>
#include <cstdint>

struct Buffer {
    size_t width, height;
    uint32_t* data;
};



uint32_t rgb_to_uint32(uint8_t r, uint8_t g, uint8_t b);

void buffer_clear(Buffer* buffer, uint32_t color);
