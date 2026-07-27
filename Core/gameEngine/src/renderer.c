#include "renderer.h"
#include "assetManager.h"
#include "ili9341.h"
#include "images.h"

#include <stdint.h>



void renderer_init(renderer_t *renderer, ili9341_t *ili9341, assetManager_t *assets, map_t *map, color_t backgroundColor){
    renderer->ili9341 = ili9341;
    renderer->assets = assets;
    renderer->backgroundColor = backgroundColor;
    renderer->mapOffset->x = map->size_tiles.width * map->tileSize_pixels;
    renderer->mapOffset->y = map->size_tiles.height * map->tileSize_pixels;
}


void renderer_clear(renderer_t *renderer, color_t color){
    ILI9341_fillScreen(renderer->ili9341, color);
}
void renderer_drawPixel(renderer_t *renderer, uint16_t x, uint16_t y, uint16_t, color_t color){
    ILI9341_drawPixel(renderer->ili9341, x, y, color);
}
void renderer_drawRectangle(renderer_t *renderer, uint16_t x, uint16_t y, uint16_t width, uint16_t height, color_t color){
    ILI9341_drawRectangle(renderer->ili9341, x, y, width, height, color);
}
void renderer_drawSprite(renderer_t *renderer, SpriteID id, uint16_t x, uint16_t y){
    
}

/*
void renderer_drawTile(renderer_t *renderer, SpriteID id, uint16_t x, uint16_t y){
    sprite_t *tileSprite = assetManager_getSprite(id); 
    image_t tileImage = {.Buffer = tileSprite->data, .Width = tileSprite->width, .Height = tileSprite->height};
    ILI9341_drawImage(renderer->ili9341, x, y, &tileImage);
}
*/

void renderer_drawMap(renderer_t *renderer){

}