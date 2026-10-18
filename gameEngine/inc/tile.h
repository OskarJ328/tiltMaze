#pragma once

#include "sprite.h"

typedef enum {
    TILE_WALL,
    TILE_FLOOR,
    TILE_HOLE,
    TILE_NULL
}TileID;

typedef enum {
    TILE_TYPE_WALL,
    TILE_TYPE_FLOOR,
    TILE_TYPE_HOLE,
    TILE_TYPE_NULL
}TileType;

typedef struct {
    TileType type;

    SpriteID spriteId;
    uint8_t flag;
}tile_t;

extern tile_t tiles[5];