#pragma once

#include "basicTypes.h"
#include "ili9341.h"
#include "assetManager.h"
#include <stdint.h>

typedef struct {
    uint8_t tileSize;
    uint8_t width_tiles;
    uint8_t height_tiles;

    const uint8_t *data;
}map_t;

typedef struct {
    ili9341_t       *ili9341;
    assetManager_t  assetManager;
    uint16_t        backgroundColor;
    map_t           *map;
}renderer_t;



void renderer_clear(renderer_t *renderer, color_t color);
void renderer_drawPixel(renderer_t *renderer, uint16_t x, uint16_t y, uint16_t, color_t color);
void renderer_drawRectangle(renderer_t *renderer, uint16_t x, uint16_t y, uint16_t width, uint16_t height, color_t color);
void renderer_drawSprite(renderer_t *renderer, SpriteID id, uint16_t x, uint16_t y);