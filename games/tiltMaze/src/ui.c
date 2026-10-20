#include "ui.h"
#include "assetManager.h"
#include "basicTypes.h"
#include "fonts.h"
#include "ili9341.h"
#include "images.h"
#include "renderer.h"
#include "sprite.h"
#include <assert.h>
#include <stdint.h>
#include <string.h>


static void ui_drawSpriteColumn(renderer_t *renderer, vector2_t startingPosition, uint8_t spriteNumber, SpriteID id){
    const sprite_t *sprite = assetManager_getSprite(renderer->assets, id);
    if(sprite == NULL){
        return;
    }
    for(uint8_t y = 0; y < spriteNumber; y++){
        vector2_t position;
        position.x = startingPosition.x;
        position.y = startingPosition.y + y * sprite->size.height; 
        renderer_drawSprite(renderer, id, position);
    }
}

static void ui_drawSpriteRow(renderer_t *renderer, vector2_t startingPosition, uint8_t spriteNumber, SpriteID id){
    const sprite_t *sprite = assetManager_getSprite(renderer->assets, id);
    if(sprite == NULL){
        return;
    }
    for(uint8_t x = 0; x < spriteNumber; x++){
        vector2_t position;
        position.x = startingPosition.x + x * sprite->size.width;
        position.y = startingPosition.y;
        renderer_drawSprite(renderer, id, position);
    }
}

void ui_drawOuterFrame(renderer_t *renderer, rectangle_t rectangle, SpriteID id){
    const sprite_t *sprite = assetManager_getSprite(renderer->assets, id);
    if(sprite == NULL){
        return;
    }
    vector2_t position;
    uint8_t spriteNumber;
    /*
    COLUMNS
    */
    spriteNumber = rectangle.size.height / sprite->size.height;
    /*
    left column
    */
    position.x = rectangle.position.x - sprite->size.width;
    position.y = rectangle.position.y;
    ui_drawSpriteColumn(renderer, position, spriteNumber, id);
    /*
    right column
    */
    position.x = rectangle.position.x + rectangle.size.width;
    position.y = rectangle.position.y;
    ui_drawSpriteColumn(renderer, position, spriteNumber, id);
    /*
    ROWS
    */
    spriteNumber = 1 + rectangle.size.width / sprite->size.width + 1;
    /*
    upper row
    */
    position.x = rectangle.position.x - sprite->size.width;
    position.y = rectangle.position.y - sprite->size.height;
    ui_drawSpriteRow(renderer, position, spriteNumber, id);
    /*
    bottom row
    */
    position.x = rectangle.position.x - sprite->size.width;
    position.y = rectangle.position.y + rectangle.size.height;
    ui_drawSpriteRow(renderer, position, spriteNumber, id);
}

void ui_drawTextInRectangle(renderer_t *renderer, rectangle_t rectangle, FontID id, char *text){
    const font_t *font = assetManager_getFont(renderer->assets, id);

    vector2_t rectangleCenter;
    rectangleCenter.x = rectangle.position.x + rectangle.size.width / 2;
    rectangleCenter.y = rectangle.position.y + rectangle.size.height / 2;
    
    uint16_t textLen_chars = strlen(text);
    Size textSize_pixels;
    textSize_pixels.width = textLen_chars * font->width;
    textSize_pixels.height = font->height;

    vector2_t position;
    position.x = rectangleCenter.x - textSize_pixels.width / 2;
    position.y = rectangleCenter.y - textSize_pixels.height / 2;

    renderer_drawText(renderer, id, text, position, ILI9341_BLACK);
}

void ui_drawButton(renderer_t *renderer, vector2_t position, buttonGraphic_t *buttonGraphic, const char *text){

}