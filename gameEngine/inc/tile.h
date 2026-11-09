#pragma once

#include "sprite.h"

typedef enum {
    TILE_WALL_1,
    TILE_WALL_2,
    TILE_WALL_3,
    TILE_FLOOR_1,
    TILE_FLOOR_2,
    TILE_FLOOR_3,
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

extern tile_t tiles[10];