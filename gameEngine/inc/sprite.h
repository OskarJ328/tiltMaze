#pragma once
#include "basicTypes.h"


typedef enum{
    SPRITE_WALL_1,
    SPRITE_WALL_2,
    SPRITE_WALL_3,  
    SPRITE_FLOOR_1,
    SPRITE_FLOOR_2,
    SPRITE_FLOOR_3,
    SPRITE_HOLE,

    SPRITE_PLAYER,
    SPRITE_BALL,
    SPRITE_GOAL,

    SPRITE_TEXT_FRAME
    
} SpriteID;

typedef struct {
    const uint16_t *data;
    Size size;
    const uint32_t *mask;
}sprite_t;

/*
-----------MASK-----------

jeden element tablicy odpowiada jednemu wierszowi spritea
jeden bit danego elementu odpowiada jednej kolumnie spritea
0 - pixel nie jest rysowany
1 - pixel jest rysowany
*/

extern sprite_t sprites[20];