#include "assetManager.h"
#include "stddef.h"
#include "ili9341.h"


#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0])) 

#define SOLID_16X16_ROW(color) \
    color, color, color, color, color, color, color, color, \
    color, color, color, color, color, color, color, color

#define WALL_ROW   WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL
#define FLOOR_ROW  WALL, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, WALL


static const uint16_t wall16x16_buffer[] = {
    SOLID_16X16_ROW(ILI9341_BLUE), SOLID_16X16_ROW(ILI9341_BLUE),
    SOLID_16X16_ROW(ILI9341_BLUE), SOLID_16X16_ROW(ILI9341_BLUE),
    SOLID_16X16_ROW(ILI9341_BLUE), SOLID_16X16_ROW(ILI9341_BLUE),
    SOLID_16X16_ROW(ILI9341_BLUE), SOLID_16X16_ROW(ILI9341_BLUE),
    SOLID_16X16_ROW(ILI9341_BLUE), SOLID_16X16_ROW(ILI9341_BLUE),
    SOLID_16X16_ROW(ILI9341_BLUE), SOLID_16X16_ROW(ILI9341_BLUE),
    SOLID_16X16_ROW(ILI9341_BLUE), SOLID_16X16_ROW(ILI9341_BLUE),
    SOLID_16X16_ROW(ILI9341_BLUE), SOLID_16X16_ROW(ILI9341_BLUE)
};

static const uint16_t floor16x16_buffer[] = {
    SOLID_16X16_ROW(ILI9341_RED), SOLID_16X16_ROW(ILI9341_RED),
    SOLID_16X16_ROW(ILI9341_RED), SOLID_16X16_ROW(ILI9341_RED),
    SOLID_16X16_ROW(ILI9341_RED), SOLID_16X16_ROW(ILI9341_RED),
    SOLID_16X16_ROW(ILI9341_RED), SOLID_16X16_ROW(ILI9341_RED),
    SOLID_16X16_ROW(ILI9341_RED), SOLID_16X16_ROW(ILI9341_RED),
    SOLID_16X16_ROW(ILI9341_RED), SOLID_16X16_ROW(ILI9341_RED),
    SOLID_16X16_ROW(ILI9341_RED), SOLID_16X16_ROW(ILI9341_RED),
    SOLID_16X16_ROW(ILI9341_RED), SOLID_16X16_ROW(ILI9341_RED)
};

static sprite_t sprites[2] = {
    [SPRITE_WALL] = {
        .data = wall16x16_buffer,
        .size = { .width = 16, .height = 16 }
    },
    [SPRITE_FLOOR] = {
        .data = floor16x16_buffer,
        .size = { .width = 16, .height = 16 }
    }
};

static font_t   fonts[2];

static const TileID exampleMapTileIds[] = {
    WALL_ROW,
    FLOOR_ROW,
    FLOOR_ROW,
    FLOOR_ROW,
    FLOOR_ROW,
    FLOOR_ROW,
    FLOOR_ROW,
    FLOOR_ROW,
    FLOOR_ROW,
    WALL_ROW
};

static const map_t    map[1] = {
    {
        .size_tiles = { .width = 10, .height = 10 },
        .tileSize_pixels = 16,
        .tileIds = exampleMapTileIds
    }
};

static tile_t   tiles[2] = {
    {
        .spriteId = SPRITE_WALL,
        .flag = 1
    },
    {
        .spriteId = SPRITE_FLOOR,
        .flag = 0
    }
};

void assetManager_init(assetManager_t *am){
    am->sprites         = sprites;
    am->spritesCount    = ARRAY_SIZE(sprites);

    am->tiles       = tiles;
    am->tileCount   = ARRAY_SIZE(tiles);

    am->fonts       = fonts;
    am->fontCount   = ARRAY_SIZE(fonts);


    am->map         = map;
    am->mapCount    = ARRAY_SIZE(map);
}

const sprite_t *assetManager_getSprite(assetManager_t *am, SpriteID id){
    if(id >= am->spritesCount){
        return NULL;
    }
    return &am->sprites[id];
}

const tile_t *assetManager_getTile(assetManager_t *am, TileID id){
    if(id >= am->tileCount){
        return NULL;
    }
    return &am->tiles[id];
}

const font_t *assetManager_getFont(assetManager_t *am, FontID id){
    if(id >= am->fontCount){
        return NULL;
    }
    return &am->fonts[id];
}

const map_t *assetManager_getMap(assetManager_t *am, MapID id){
    if(id >= am->mapCount){
        return NULL;
    }
    return &am->map[id];
}

#undef SOLID_16X16_ROW
#undef WALL_ROW
#undef FLOOR_ROW