#pragma once

#include "assetManager.h"
#include "basicTypes.h"
#include "fonts.h"
#include "renderer.h"
#include "sprite.h"

typedef struct{
    FontID fontID;
    color_t fontColor;
    SpriteID background;
    SpriteID frame;
    SpriteID pointer;
    rectangle_t rectangle;
}buttonGraphic_t;

typedef struct{
    SpriteID frameID;
    SpriteID centerID;
}screenBackground_t;

buttonGraphic_t ui_createButtonGraphic(FontID fontID, color_t fontColor, SpriteID backgroundID, SpriteID frameID, SpriteID pointerID, rectangle_t rectangle);
void ui_drawButton(renderer_t *renderer, buttonGraphic_t buttonGraphic, const char *text);
void ui_drawButtonPointer(renderer_t *renderer, buttonGraphic_t buttonGraphic);
void ui_clearButtonPointer(renderer_t* renderer, buttonGraphic_t buttonGraphic, screenBackground_t screenBackground);
void ui_drawOuterFrame(renderer_t *renderer, rectangle_t rectangle, SpriteID id);
void ui_drawTextInRectangle(renderer_t *renderer, rectangle_t rectangle, FontID id, color_t color, const char *text);
void ui_drawScreenBackground(renderer_t *renderer, screenBackground_t screenBackground);