#pragma once

#include "assetManager.h"
#include "basicTypes.h"
#include "fonts.h"
#include "renderer.h"
#include "sprite.h"
#include "tile.h"
#include <stdint.h>

typedef struct{
    FontID fontID;
    color_t fontColor;
    SpriteID background;
    SpriteID frame;
    SpriteID pointer;
    rectangle_t rectangle;
}buttonGraphic_t;

typedef struct {
    FontID fontID;
    color_t defaultColor;
    color_t pointedColor;
    SpriteID textFrameID;
    rectangle_t rectangle;

    char text[32];
}ButtonGraphic;

void ui_createButtonGraphic(ButtonGraphic *buttonGraphic, FontID fontID, color_t defautlColor, color_t pointedColor, SpriteID textFrameID, vector2_t position);
void ui_drawButton(renderer_t *renderer, ButtonGraphic *buttonGraphic, const char *text);
void ui_pointAtButton(renderer_t *renderer, ButtonGraphic *buttonGraphic);
void ui_unpointAtButton(renderer_t* renderer, ButtonGraphic *buttonGraphic);
void ui_drawOuterFrame(renderer_t *renderer, rectangle_t rectangle, SpriteID id);
void ui_drawTextInRectangle(renderer_t *renderer, rectangle_t rectangle, FontID id, color_t color, const char *text);
void ui_drawScreenBackground(renderer_t *renderer, SpriteID *sprites, uint8_t spritesNumber, uint8_t *spritesThresholds, uint8_t thresholdsNumber);