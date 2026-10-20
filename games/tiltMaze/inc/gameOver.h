#pragma once

#include "assetManager.h"
#include "input.h"
#include "map.h"
#include "renderer.h"

typedef struct{
    input_t *input;
    assetManager_t *assets;
    
    map_t *map;
    bool wasScreenCleared;
}gameOver_t;

typedef enum{
    GAME_OVER_ACTION_NONE,
    GAME_OVER_ACTION_RESUME
}gameOverAction_t; 

void gameOver_init(gameOver_t *gameOver, input_t *input, assetManager_t *assets, MapID mapID);
gameOverAction_t gameOver_update(gameOver_t *gameOver);
void gameOver_render(gameOver_t *gameOver, renderer_t *renderer);