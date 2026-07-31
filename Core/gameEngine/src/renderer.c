#include "renderer.h"
#include "assetManager.h"
#include "basicTypes.h"
#include "ili9341.h"
#include "map.h"
#include "sprite.h"
#include "tile.h"
#include <stddef.h>
#include <stdint.h>
#include <string.h>

void renderer_init(renderer_t *renderer, ili9341_t *ili9341, assetManager_t *assets, const map_t *map, color_t backgroundColor){
    renderer->ili9341 = ili9341;
    renderer->assets = assets;
    renderer->backgroundColor = backgroundColor;

    
    ILI9341_init(renderer->ili9341);
    ILI9341_fillScreen(renderer->ili9341, renderer->backgroundColor);
    ILI9341_swapAxes(renderer->ili9341);
    ILI9341_invertAxis(renderer->ili9341, invertBoth);
    renderer->mapOffset.x = (ili9341->width - map->size_tiles.width * map->tileSize_pixels) / 2;
    renderer->mapOffset.y = (ili9341->height - map->size_tiles.height * map->tileSize_pixels) / 2;
}

void renderer_setBackgroundColor(renderer_t *renderer, color_t color){
    renderer->backgroundColor = color;
}

void renderer_drawPixel(renderer_t *renderer, vector2_t position, color_t color){
    ILI9341_drawPixel(renderer->ili9341, position.x, position.y, color);
}

void renderer_drawRectangle(renderer_t *renderer, rectangle_t rectangle, color_t color){
    ILI9341_drawRectangle(renderer->ili9341, rectangle.position.x, rectangle.position.y, rectangle.size.width, rectangle.size.height, color);
}

void renderer_drawSprite(renderer_t *renderer, SpriteID id, vector2_t position){
    const sprite_t *sprite = assetManager_getSprite(renderer->assets, id);
    if(sprite == NULL){
        return;
    }
    image_t spriteImage = {sprite->data, sprite->size.width, sprite->size.height};
    ILI9341_drawImage(renderer->ili9341, position.x, position.y, &spriteImage);
}

void renderer_drawTile(renderer_t *renderer, TileID id, vector2_t position){
    const tile_t *tile = assetManager_getTile(renderer->assets, id);
    if(tile == NULL){
        return;
    }
    renderer_drawSprite(renderer, tile->spriteId, position);
}

void renderer_clear(renderer_t *renderer, color_t color){
    ILI9341_fillScreen(renderer->ili9341, color);
}

void renderer_drawMap(renderer_t *renderer, const map_t *map){
    if(map == NULL){
        return;
    }
    for(uint8_t y = 0; y < map->size_tiles.height; y++){
        for(uint8_t x = 0; x < map->size_tiles.width; x++){
            vector2_t position;
            position.x = renderer->mapOffset.x + x * map->tileSize_pixels;
            position.y = renderer->mapOffset.y + y * map->tileSize_pixels;
            uint16_t mapIdx = y * map->size_tiles.width + x;
            renderer_drawTile(renderer, map->tileIds[mapIdx], position);
        }
    }
}

void renderer_drawText(renderer_t *renderer, FontID id, const char *text, vector2_t position, color_t color){
    const font_t *font = assetManager_getFont(renderer->assets, id);
    if(font == NULL){
        return;
    }
    ILI9341_writeString(renderer->ili9341, position.x, position.y, font, text, strlen(text), color, renderer->backgroundColor);
}



/*
1.  licze wpsolrzedne lewego gornego rogu
2.  wyznaczam zakres kafelkow jaki obejmuje object
3.  skopiowanie kafelkow do bufora
4.  nalozenie object na kafelki
5.  stworzenie spritea i narysowanie go
*/
// position - pozycja srodka obiektu w pixelach wedlug ukladu wspolrzednych mapy

void renderer_drawMovingObject(renderer_t *renderer, SpriteID spriteId, MapID mapId, vector2_t position){
    const sprite_t *object_sprite = assetManager_getSprite(renderer->assets, spriteId);
    const map_t *map = assetManager_getMap(renderer->assets, mapId);

    if(object_sprite == NULL || map == NULL){
        return;
    }
    vector2_t object_TopLeft_pxl;
    object_TopLeft_pxl.x = position.x - object_sprite->size.width / 2;
    object_TopLeft_pxl.y = position.y - object_sprite->size.height / 2;
    uint16_t leftTileIdx    = object_TopLeft_pxl.x / map->tileSize_pixels;
    uint16_t rightTileIdx   = (object_TopLeft_pxl.x + object_sprite->size.width - 1) / map->tileSize_pixels;
    uint16_t topTileIdx     = object_TopLeft_pxl.y / map->tileSize_pixels;
    uint16_t bottomTileIdx  = (object_TopLeft_pxl.y + object_sprite->size.height - 1) / map->tileSize_pixels;

    vector2_t returnSpriteOffest_pxl = {.x = leftTileIdx * map->tileSize_pixels, .y = topTileIdx * map->tileSize_pixels};
    uint16_t returnSpriteData[16 * 16 * 4]; //rezerwuje miejsce na 4 tile o wymiarach 16x16 pxl

    vector2_t objectInReturnSprite = {.x = object_TopLeft_pxl.x - returnSpriteOffest_pxl.x, .y = object_TopLeft_pxl.y - returnSpriteOffest_pxl.y};

    Size returnSpriteSize_tiles = {rightTileIdx - leftTileIdx + 1, bottomTileIdx - topTileIdx + 1};
    Size returnSpriteSize_pxl;
    returnSpriteSize_pxl.width  = returnSpriteSize_tiles.width * map->tileSize_pixels;
    returnSpriteSize_pxl.height = returnSpriteSize_tiles.height * map->tileSize_pixels;
    for(uint16_t y = topTileIdx; y <= bottomTileIdx; y++){
        for(uint16_t x = leftTileIdx; x <= rightTileIdx; x++){
            TileID id = map->tileIds[y * map->size_tiles.width + x];
            const tile_t *tile = assetManager_getTile(renderer->assets, id);
            const sprite_t *sprite = assetManager_getSprite(renderer->assets, tile->spriteId);

            /*
            DOKONCZYC ALGORYTM UZUPELNIAJACY TABLICE RETURN_SPRITE_DATA
            */

        }
    }

    for(uint16_t y = 0; y < object_sprite->size.height; y++){
        for(uint16_t x = 0; x < object_sprite->size.width; x++){
            if(object_sprite->mask[y] & (1 << x)){
                uint16_t idx = (objectInReturnSprite.y + y) * returnSpriteSize_pxl.width + objectInReturnSprite.x + x ;
                returnSpriteData[idx] = object_sprite->data[y * object_sprite->size.width + x];
            }
        }
    }
    image_t spriteImage = {returnSpriteData, returnSpriteSize_pxl.width, returnSpriteSize_pxl.height};
    ILI9341_drawImage(renderer->ili9341, renderer->mapOffset.x + returnSpriteOffest_pxl.x, renderer->mapOffset.y + returnSpriteOffest_pxl.y, &spriteImage);
}
