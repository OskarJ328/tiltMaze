#pragma once

#include "ili9341.h"
#include "assetManager.h"



typedef struct {
    ili9341_t       *ili9341;
    assetManager_t  *assets;
    color_t         backgroundColor;
}renderer_t;


void renderer_init(renderer_t *renderer, ili9341_t *ili9341, assetManager_t *assets, color_t backgroundColor);
void renderer_setBackgroundColor(renderer_t *renderer, color_t color);

void renderer_clear(renderer_t *renderer, color_t color);
void renderer_drawPixel(renderer_t *renderer, vector2_t position, color_t color);
void renderer_drawRectangle(renderer_t *renderer, rectangle_t rectangle, color_t color);
void renderer_drawSprite(renderer_t *renderer, SpriteID id, vector2_t position);
void renderer_drawTile(renderer_t *renderer, TileID id, vector2_t position);
void renderer_drawMap(renderer_t *renderer, MapID id);
void renderer_drawMovingObject(renderer_t *renderer, SpriteID spriteId, MapID mapId, vector2_t position);

void renderer_drawText(renderer_t *renderer, FontID id, const char *text, vector2_t position, color_t color);