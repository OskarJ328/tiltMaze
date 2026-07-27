#pragma once

#include <stdint.h>
#include "fonts.h"
#include "basicTypes.h"
#include "map.h"
#include "tile.h"




typedef struct {
    const sprite_t *sprites;
    uint16_t spritesCount;

    const font_t *fonts;
    uint16_t fontCount;

    const map_t *map;
    uint8_t mapCount;

    const tile_t *tiles;
    uint16_t tileCount;
}assetManager_t;

void assetManager_init(assetManager_t *am);
sprite_t *assetManager_getSprite(assetManager_t *am, SpriteID id);
tile_t *assetManager_getTile(assetManager_t *am, TileID id);
font_t *assetManager_getFont(assetManager_t *am, FontID id);
map_t *assetManager_getMap(assetManager_t *am, MapID id);