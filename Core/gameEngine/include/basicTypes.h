#pragma once

#include "stdint.h"

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0])) 

typedef uint16_t color_t;

typedef struct {
    uint16_t width;
    uint16_t height;
}Size;

typedef struct {
    int16_t x;
    int16_t y;
} vector2_t;

typedef struct {
    float x;
    float y;
    float z;
} floatVector3_t;


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




