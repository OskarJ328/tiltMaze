#include "assetManager.h"
#include "fonts.h"


static sprite_t sprites[];
static font_t   fonts[];


sprite_t *assetManager_getSprite(SpriteID id){
    return &sprites[id];
}
font_t * assetManager_getFont(FontID id){
    return &fonts[id];
}

