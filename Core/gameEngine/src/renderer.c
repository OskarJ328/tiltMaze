#include "renderer.h"
#include "ili9341.h"

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