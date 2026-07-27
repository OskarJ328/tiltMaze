#pragma once
#include "basicTypes.h"


typedef enum{
    SPRITE_PLAYER,
    SPRITE_WALL,
    SPRITE_BALL,
    SPRITE_GOAL,
    SPRITE_FLOOR
} SpriteID;

typedef struct {
    const uint16_t *data;
    Size size;
}sprite_t;