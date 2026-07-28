#pragma once
#include "basicTypes.h"


typedef enum{
    SPRITE_WALL,  
    SPRITE_FLOOR,

    SPRITE_PLAYER,
    SPRITE_BALL,
    SPRITE_GOAL
    
} SpriteID;

typedef struct {
    const uint16_t *data;
    Size size;
}sprite_t;