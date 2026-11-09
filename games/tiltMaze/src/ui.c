#include "assetManager.h"
#include "basicTypes.h"
#include "sprite.h"
#include <stdint.h>
#include <stdlib.h>
#define POINTER_OFFSET_PXL 10

#include "ui.h"
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

void ui_createButtonGraphic(ButtonGraphic *buttonGraphic, FontID fontID, color_t defautlColor, color_t pointedColor, SpriteID textFrameID, vector2_t position){
    buttonGraphic->fontID = fontID;
    buttonGraphic->defaultColor = defautlColor;
    buttonGraphic->pointedColor = pointedColor;
    buttonGraphic->textFrameID = textFrameID;
    buttonGraphic->rectangle.position = position;
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

void ui_drawButton(renderer_t *renderer, ButtonGraphic *buttonGraphic, const char *text){
    const sprite_t *textFrame = assetManager_getSprite(renderer->assets, buttonGraphic->textFrameID);
    if(textFrame == NULL){
        return;
    }
    
    strcpy(buttonGraphic->text, text);
    buttonGraphic->rectangle.size.width = textFrame->size.width;
    buttonGraphic->rectangle.size.height = textFrame->size.height;
    
    renderer_drawSprite(renderer, buttonGraphic->textFrameID, buttonGraphic->rectangle.position);
    ui_drawTextInRectangle(renderer, buttonGraphic->rectangle, buttonGraphic->fontID, buttonGraphic->defaultColor, text);
}

void ui_pointAtButton(renderer_t *renderer, ButtonGraphic *buttonGraphic){
    ui_drawTextInRectangle(renderer, buttonGraphic->rectangle, buttonGraphic->fontID, buttonGraphic->pointedColor, buttonGraphic->text);
}

void ui_unpointAtButton(renderer_t *renderer, ButtonGraphic *buttonGraphic){
    ui_drawTextInRectangle(renderer, buttonGraphic->rectangle, buttonGraphic->fontID, buttonGraphic->defaultColor, buttonGraphic->text);
}

void ui_drawScreenBackground(renderer_t *renderer, SpriteID *sprites, uint8_t spritesNumber, uint8_t *spritesThresholds, uint8_t thresholdsNumber){
    if(spritesNumber != thresholdsNumber){
        return;
    }
    const sprite_t *sprite = assetManager_getSprite(renderer->assets, sprites[0]);
    Size screen_sprites;
    screen_sprites.width = renderer->ili9341->width / sprite->size.width;
    screen_sprites.height = renderer->ili9341->height / sprite->size.height;

    for(uint8_t x = 0; x < screen_sprites.width; x++){
        for(uint8_t y = 0; y < screen_sprites.height; y++){
            vector2_t position;
            position.x = x * sprite->size.width;
            position.y = y * sprite->size.height;
            
            SpriteID id;
            
            uint8_t randomValue = rand() % 100;
            for(uint8_t i = 0; i < spritesNumber; i++){
                if(randomValue < spritesThresholds[i]){
                    id = sprites[i];
                    break;
                }
            }

            renderer_drawSprite(renderer, id, position);
        }
    }
}