#pragma once

#include "basicTypes.h"
#include "tile.h"
#include <stdint.h>

typedef enum {
    MAP_FANTASY,
    MAP_SCI_FI,
    MAP_GOTH
}MapID;

typedef struct {
    Size size_tiles;
    uint8_t tileSize_pixels;
    const TileID *tileIds;
    vector2_t offset;
}map_t;

void map_CountOffset(map_t *map, uint16_t screenWidth, uint16_t screenHeight);

extern map_t maps[2]; 