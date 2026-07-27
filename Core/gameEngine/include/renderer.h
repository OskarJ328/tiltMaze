#pragma once

#include "basicTypes.h"
#include "ili9341.h"
#include "assetManager.h"
#include "map.h"
#include <stdint.h>



typedef struct {
    ili9341_t       *ili9341;
    assetManager_t  *assets;
    vector2_t       *mapOffset;
    color_t         backgroundColor;
}renderer_t;


void renderer_init(renderer_t *renderer, ili9341_t *ili9341, assetManager_t *assets, map_t *map, color_t backgroundColor);
void renderer_clear(renderer_t *renderer, color_t color);
void renderer_drawPixel(renderer_t *renderer, uint16_t x, uint16_t y, uint16_t, color_t color);
void renderer_drawRectangle(renderer_t *renderer, uint16_t x, uint16_t y, uint16_t width, uint16_t height, color_t color);
void renderer_drawSprite(renderer_t *renderer, SpriteID id, uint16_t x, uint16_t y);
void renderer_drawTile(renderer_t *renderer, SpriteID id, uint16_t x, uint16_t y);
void renderer_drawMap(renderer_t *renderer);