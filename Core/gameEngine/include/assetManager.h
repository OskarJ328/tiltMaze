#pragma once

#include "fonts.h"
#include "map.h"



typedef struct {
    const sprite_t *sprites;
    uint16_t spritesCount;

    const font_t *fonts;
    uint16_t fontCount;

    const map_t *maps;
    uint8_t mapCount;

    const tile_t *tiles;
    uint16_t tileCount;
}assetManager_t;

void assetManager_init(assetManager_t *am, sprite_t *sprites, tile_t *tiles, font_t *fonts, map_t *maps);
const sprite_t *assetManager_getSprite(assetManager_t *am, SpriteID id);
const tile_t *assetManager_getTile(assetManager_t *am, TileID id);
const font_t *assetManager_getFont(assetManager_t *am, FontID id);
const map_t *assetManager_getMap(assetManager_t *am, MapID id);