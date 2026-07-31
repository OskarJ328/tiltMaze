#pragma once

#include "tile.h"

typedef enum {
    FANTASY,
    SCI_FI,
    GOTH
}MapID;

typedef struct {
    Size size_tiles;
    uint8_t tileSize_pixels;
    const TileID *tileIds;
}map_t;

extern map_t maps[2]; 