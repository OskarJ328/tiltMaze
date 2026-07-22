#include "assetManager.h"
#include "fonts.h"


static sprite_t sprites[];
static font_t   fonts[];
static map_t    map[];

sprite_t *assetManager_getSprite(SpriteID id){
    return &sprites[id];
}
font_t *assetManager_getFont(FontID id){
    return &fonts[id];
}

map_t *assetManager_getMap(){
    return &map;
}