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
    const uint16_t *mask;
}sprite_t;

/*
-----------MASK-----------

jeden element tablicy odpowiada jednemu wierszowi spritea
jeden bit danego elementu odpowiada jednej kolumnie spritea
0 - pixel nie jest rysowany
1 - pixel jest rysowany
*/

extern sprite_t sprites[20];