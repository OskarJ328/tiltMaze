#pragma once

#include "stdint.h"

typedef uint16_t color_t;

typedef struct {
    uint16_t width;
    uint16_t height;
}Size;

typedef struct {
    int16_t x;
    int16_t y;
} vector2_t;

typedef struct{
    vector2_t   position;
    Size        size;
} rectangle_t;

typedef enum {
    DIR_UP,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT
} directions_t;




