#include "renderer.h"
#include "ili9341.h"
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
