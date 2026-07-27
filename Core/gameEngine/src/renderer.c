#include "renderer.h"
#include "assetManager.h"
#include "ili9341.h"
#include "images.h"
#include <stdint.h>


static void countMapOffset(renderer_t *renderer){
    map_t *map = assetManager_getMap();
    uint16_t mapWidth_pxl = 0;
    uint16_t mapHeight_pxl = 0;
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

void renderer_drawTile(renderer_t *renderer, SpriteID id, uint16_t x, uint16_t y){
    sprite_t *tileSprite = assetManager_getSprite(id); 
    image_t tileImage = {.Buffer = tileSprite->data, .Width = tileSprite->width, .Height = tileSprite->height};
    ILI9341_drawImage(renderer->ili9341, x, y, &tileImage);
}

void renderer_drawMap(renderer_t *renderer){

}