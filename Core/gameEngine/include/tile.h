#pragma once

#include "sprite.h"

typedef enum {
    WALL,
    FLOOR
}TileID;

typedef struct {
    SpriteID spriteId;
    uint8_t flag;
}tile_t;