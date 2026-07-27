#pragma once

#include "stdint.h"

typedef uint16_t color_t;

typedef struct {
    width,
    height
}Size;

typedef struct {
    Size size_tiles;
    Size size_margins_pixels;
    const uint8_t *tileIds;
}map_t;

typedef struct {
    int16_t x;
    int16_t y;
} vector2_t;

typedef struct{
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
} rectangle_t;

typedef enum {
    DIR_UP,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT
} directions_t;

