#define POINTER_OFFSET_PXL 10

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

buttonGraphic_t ui_createButtonGraphic(FontID fontID, color_t fontColor, SpriteID backgroundID, SpriteID frameID, SpriteID pointerID, rectangle_t rectangle){
    buttonGraphic_t btnGrph;
    btnGrph.fontID      = fontID;
    btnGrph.fontColor   = fontColor;
    btnGrph.background  = backgroundID;
    btnGrph.frame       = frameID;
    btnGrph.pointer     = pointerID;
    btnGrph.rectangle   = rectangle;
    return btnGrph;
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

void ui_drawTextInRectangle(renderer_t *renderer, rectangle_t rectangle, FontID id, color_t fontColor, const char *text){
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

    renderer_drawText_noBackgroung(renderer, id, text, position, fontColor);
}

static void ui_drawSpriteRectangle(renderer_t *renderer, rectangle_t rectangle, SpriteID id){
    const sprite_t *sprite = assetManager_getSprite(renderer->assets, id);
    if(sprite == NULL){
        return;
    }

    Size rectangleSize_sprites;
    rectangleSize_sprites.width = rectangle.size.width / sprite->size.width;
    rectangleSize_sprites.height = rectangle.size.height / sprite->size.height;

    for(uint8_t x = 0; x < rectangleSize_sprites.width; x++){
        vector2_t position;
        position.x = rectangle.position.x + x * sprite->size.width;
        position.y = rectangle.position.y;

        ui_drawSpriteColumn(renderer, position, rectangleSize_sprites.height, id);
    }
}

void ui_drawButton(renderer_t *renderer, buttonGraphic_t buttonGraphic, const char *text){
    ui_drawOuterFrame(renderer, buttonGraphic.rectangle, buttonGraphic.frame);
    ui_drawSpriteRectangle(renderer, buttonGraphic.rectangle, buttonGraphic.background);
    ui_drawTextInRectangle(renderer, buttonGraphic.rectangle, buttonGraphic.fontID, buttonGraphic.fontColor, text);
}

void ui_drawButtonPointer(renderer_t *renderer, buttonGraphic_t buttonGraphic){
    vector2_t position;
    position.x = buttonGraphic.rectangle.position.x - POINTER_OFFSET_PXL;
    position.y = buttonGraphic.rectangle.position.y + buttonGraphic.rectangle.size.height / 2;
    
    renderer_drawSpriteWithAlphaMask(renderer, buttonGraphic.pointer, position);
}

void ui_clearButtonPointer(renderer_t *renderer, buttonGraphic_t buttonGraphic, screenBackground_t screenBackground){
    vector2_t position;
    //position.x = buttonGraphic.position    
}

void ui_drawScreenBackground(renderer_t *renderer, screenBackground_t screenBackground){
    const sprite_t *frame = assetManager_getSprite(renderer->assets, screenBackground.frameID);
    if(frame == NULL){
        return;
    }
    rectangle_t rectangle;
    rectangle.size.width = renderer->ili9341->width - (frame->size.width * 2);
    rectangle.size.height = renderer->ili9341->height - (frame->size.height * 2);
    rectangle.position.x = frame->size.width;
    rectangle.position.y = frame->size.height;
    ui_drawOuterFrame(renderer, rectangle, screenBackground.frameID);
    ui_drawSpriteRectangle(renderer, rectangle, screenBackground.centerID); 
}