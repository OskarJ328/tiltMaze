#pragma once

#include "basicTypes.h"
#include "sprite.h"

typedef enum {
    FLOOR,
    HOLE,
    END
}TileID;

typedef struct {
    SpriteID spriteId;
    uint8_t flag;
}tile_t;