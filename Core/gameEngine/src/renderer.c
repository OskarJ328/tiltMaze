#define MAX_TILES 4
#define MAX_TILE_SIZE_PXL 16

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


void renderer_init(renderer_t *renderer, ili9341_t *ili9341, assetManager_t *assets, color_t backgroundColor){
    renderer->ili9341 = ili9341;
    renderer->assets = assets;
    renderer->backgroundColor = backgroundColor;

    
    ILI9341_init(renderer->ili9341);
    ILI9341_fillScreen(renderer->ili9341, renderer->backgroundColor);
    ILI9341_swapAxes(renderer->ili9341);
    ILI9341_invertAxis(renderer->ili9341, invertBoth);

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

void renderer_drawMap(renderer_t *renderer, MapID id){
    map_t *map = assetManager_getMap(renderer->assets, id);
    if(map == NULL){
        return;
    }
    map_CountOffset(map, renderer->ili9341->width, renderer->ili9341->height);
    for(uint8_t y = 0; y < map->size_tiles.height; y++){
        for(uint8_t x = 0; x < map->size_tiles.width; x++){
            vector2_t position;
            position.x = map->offset.x + x * map->tileSize_pixels;
            position.y = map->offset.y + y * map->tileSize_pixels;
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
    map_t *map = assetManager_getMap(renderer->assets, mapId);
    if(object_sprite == NULL || map == NULL){
        return;
    }
    map_CountOffset(map, renderer->ili9341->width, renderer->ili9341->height);
    
    vector2_t object_TopLeft_pxl;
    object_TopLeft_pxl.x = position.x - object_sprite->size.width / 2;
    object_TopLeft_pxl.y = position.y - object_sprite->size.height / 2;

    vector2_t leftTopTileIdx;
    leftTopTileIdx.x = object_TopLeft_pxl.x / map->tileSize_pixels;
    leftTopTileIdx.y = object_TopLeft_pxl.y / map->tileSize_pixels;
    
    vector2_t rightBottomTileIdx;
    rightBottomTileIdx.x = (object_TopLeft_pxl.x + object_sprite->size.width - 1) / map->tileSize_pixels;
    rightBottomTileIdx.y = (object_TopLeft_pxl.y + object_sprite->size.height - 1) / map->tileSize_pixels;

    vector2_t returnSpriteOffest_pxl;
    returnSpriteOffest_pxl.x = leftTopTileIdx.x * map->tileSize_pixels; 
    returnSpriteOffest_pxl.y = leftTopTileIdx.y * map->tileSize_pixels;

    vector2_t objectInReturnSprite;
    objectInReturnSprite.x = object_TopLeft_pxl.x - returnSpriteOffest_pxl.x;
    objectInReturnSprite.y = object_TopLeft_pxl.y - returnSpriteOffest_pxl.y;

    Size returnSpriteSize_tiles;
    returnSpriteSize_tiles.width = rightBottomTileIdx.x - leftTopTileIdx.x + 1;
    returnSpriteSize_tiles.height = rightBottomTileIdx.y - leftTopTileIdx.y + 1;
    
    Size returnSpriteSize_pxl;
    returnSpriteSize_pxl.width  = returnSpriteSize_tiles.width * map->tileSize_pixels;
    returnSpriteSize_pxl.height = returnSpriteSize_tiles.height * map->tileSize_pixels;
    
    uint16_t returnSpriteData[MAX_TILE_SIZE_PXL * MAX_TILE_SIZE_PXL * MAX_TILES];

    for(uint16_t y = leftTopTileIdx.y; y <= rightBottomTileIdx.y; y++){
        for(uint16_t x = leftTopTileIdx.x; x <= rightBottomTileIdx.x; x++){
            TileID id = map->tileIds[y * map->size_tiles.width + x];
            const tile_t *tile = assetManager_getTile(renderer->assets, id);
            const sprite_t *sprite = assetManager_getSprite(renderer->assets, tile->spriteId);

            vector2_t tileInReturnSprite;
            tileInReturnSprite.x = (x - leftTopTileIdx.x) * map->tileSize_pixels;
            tileInReturnSprite.y = (y - leftTopTileIdx.y) * map->tileSize_pixels;

            for(uint16_t py = 0; py < map->tileSize_pixels; py++){
                for(uint16_t px = 0; px < map->tileSize_pixels; px++){
                    uint16_t bufferX = tileInReturnSprite.x + px;
                    uint16_t bufferY = tileInReturnSprite.y + py;

                    uint32_t bufferIdx = bufferY * returnSpriteSize_pxl.width + bufferX;
                    uint32_t spriteIdx = py * sprite->size.width + px;

                    returnSpriteData[bufferIdx] = sprite->data[spriteIdx];
                }
            }

        }
    }

    for(uint16_t y = 0; y < object_sprite->size.height; y++){
        for(uint16_t x = 0; x < object_sprite->size.width; x++){
            if(object_sprite->mask[y] & (1 << (31 - x))){
                uint16_t bufferX = objectInReturnSprite.x + x;
                uint16_t bufferY = objectInReturnSprite.y + y;

                uint16_t returnSpriteIdx = bufferY * returnSpriteSize_pxl.width + bufferX;
                uint16_t objectSpriteIdx = y * object_sprite->size.width + x;

                returnSpriteData[returnSpriteIdx] = object_sprite->data[objectSpriteIdx];
            }
        }
    }
    image_t returnSpriteImage = {returnSpriteData, returnSpriteSize_pxl.width, returnSpriteSize_pxl.height};
    ILI9341_drawImage(renderer->ili9341, map->offset.x + returnSpriteOffest_pxl.x, map->offset.y + returnSpriteOffest_pxl.y, &returnSpriteImage);
}
