#pragma once
#include "assetManager.h"
#include "gameOver.h"
#include "gameplay.h"
#include "input.h"
#include "menu.h"
#include "pauseMenu.h"
#include "renderer.h"
#include <stdint.h>



typedef enum{
    GAME_STATE_MENU,
    GAME_STATE_PLAYING,
    GAME_STATE_PAUSED,
    GAME_STATE_LEVEL_SELECT,
    GAME_STATE_SETTINGS,
    GAME_STATE_LEVEL_COMPLETED,
    GAME_STATE_GAME_OVER
}gameState_t;

typedef struct{
    gameState_t state;
    
    menu_t menu;
    gameplay_t gameplay;
    pauseMenu_t pauseMenu;
    gameOver_t gameOver;

    input_t *input;
    renderer_t *renderer;
    assetManager_t *assets;
}game_t;

void game_init(game_t *game, input_t *input, renderer_t *renderer, assetManager_t *assets);
void game_update(game_t *game, uint32_t deltaTime_ms);
void game_render(game_t *game);