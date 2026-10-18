#pragma once

#include "assetManager.h"
#include "input.h"
#include "map.h"
#include "renderer.h"

typedef struct{
    input_t *input;
    assetManager_t *assets;
    
    map_t *map;
}gameOver_t;

void gameOver_init(gameOver_t *gameOver, input_t *input, assetManager_t *assets, MapID mapID);
void gameOver_render(gameOver_t *gameOver, renderer_t *renderer);