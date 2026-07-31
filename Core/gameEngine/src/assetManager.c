#include "assetManager.h"
#include "fonts.h"
#include "sprite.h"
#include "stddef.h"
#include "ili9341.h"
#include "tile.h"


#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0])) 

void assetManager_init(assetManager_t *am){
    am->sprites         = sprites;
    am->spritesCount    = ARRAY_SIZE(sprites);

    am->tiles           = tiles;
    am->tileCount       = ARRAY_SIZE(tiles);

    am->fonts           = fonts;
    am->fontCount       = ARRAY_SIZE(fonts);


    am->maps            = maps;
    am->mapCount        = ARRAY_SIZE(maps);
}

const sprite_t *assetManager_getSprite(assetManager_t *am, SpriteID id){
    if(id >= am->spritesCount){
        return NULL;
    }
    return &am->sprites[id];
}

const tile_t *assetManager_getTile(assetManager_t *am, TileID id){
    if(id >= am->tileCount){
        return NULL;
    }
    return &am->tiles[id];
}

const font_t *assetManager_getFont(assetManager_t *am, FontID id){
    if(id >= am->fontCount){
        return NULL;
    }
    return &am->fonts[id];
}

map_t *assetManager_getMap(assetManager_t *am, MapID id){
    if(id >= am->mapCount){
        return NULL;
    }
    return &am->maps[id];
}