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
    uint16_t x;
    uint16_t y;
    Size size;
} rectangle_t;

typedef enum {
    DIR_UP,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT
} directions_t;

typedef enum{
    FONT_MENU,
    FONT_PAUSE,
    FONT_GAME
} FontID;


