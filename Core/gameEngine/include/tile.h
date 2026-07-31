#pragma once

#include "sprite.h"

typedef enum {
    TILE_WALL,
    TILE_FLOOR
}TileID;

typedef struct {
    SpriteID spriteId;
    uint8_t flag;
}tile_t;

extern tile_t tiles[5];