#pragma once

#include "assetManager.h"
#include "basicTypes.h"
#include "fonts.h"
#include "renderer.h"
#include "sprite.h"

typedef struct{
    font_t *font;
    color_t fontColor;
    SpriteID background;
    SpriteID frame;
}buttonGraphic_t;

void ui_drawButton(renderer_t *renderer, vector2_t position, buttonGraphic_t *buttonGraphic, const char *text);
void ui_drawOuterFrame(renderer_t *renderer, rectangle_t rectangle, SpriteID id);
void ui_drawTextInRectangle(renderer_t *renderer, rectangle_t rectangle, FontID id, char *text);