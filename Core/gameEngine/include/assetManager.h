#pragma once

#include <stdint.h>
#include "fonts.h"


typedef enum{
    SPRITE_PLAYER,
    SPRITE_WALL,
    SPRITE_BALL,
    SPRITE_GOAL,
    SPRITE_FLOOR
} SpriteID;

typedef enum{
    FONT_MENU,
    FONT_PAUSE,
    FONT_GAME
} FontID;

typedef struct {
    const uint16_t *data;

    uint16_t width;
    uint16_t height;
}sprite_t;

typedef struct {
    const sprite_t *sprites;
    uint16_t spritesCount;

    const font_t *fonts;
    uint16_t fontCount;

    const map_t *map;
}assetManager_t;

sprite_t *assetManager_getSprite(SpriteID id);
font_t *assetManager_getFont(FontID id);
map_t *assetManager_getMap();