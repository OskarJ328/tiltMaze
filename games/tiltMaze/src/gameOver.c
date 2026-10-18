#include "gameOver.h"
#include "assetManager.h"
#include "map.h"
#include "renderer.h"

void gameOver_init(gameOver_t *gameOver, input_t *input, assetManager_t *assets, MapID mapID){
    gameOver->input = input;
    gameOver->assets = assets;
    map_t *map = assetManager_getMap(assets, mapID);
    gameOver->map = map;
}

void gameOver_render(gameOver_t *gameOver, renderer_t *renderer){
    renderer_drawMap(renderer, gameOver->map->id);
}