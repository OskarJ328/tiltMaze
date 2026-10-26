#include "game.h"
#include "assetManager.h"
#include "gameOver.h"
#include "gameplay.h"
#include "ili9341.h"
#include "input.h"
#include "map.h"
#include "menu.h"
#include "renderer.h"
#include "sprite.h"

void game_init(game_t *game, input_t *input, renderer_t *renderer, assetManager_t *assets){
    game->input = input;
    game->renderer = renderer;
    game->mapID = MAP_FANTASY;
    game->state = GAME_STATE_MENU;

    menu_init(&game->menu, input, assets);
    gameplay_init(&game->gameplay, input, assets, game->mapID, SPRITE_BALL);
    gameOver_init(&game->gameOver, input, assets, game->mapID);
    //renderer_drawMap(renderer, MAP_FANTASY);
}

void game_update(game_t *game, uint32_t deltaTime_ms){
    gameplayAction_t gameplayAction;
    menuAction_t menuAction;
    gameOverAction_t gameOverAction;
    switch(game->state){
    case GAME_STATE_MENU:
        menuAction = menu_update(&game->menu);
        if(menuAction == MENU_ACTION_NEW_GAME){
            game->state = GAME_STATE_LAUNCHING;
        }
        break;
    case GAME_STATE_LAUNCHING:
        gameplay_start(&game->gameplay);
        game->state = GAME_STATE_PLAYING;
        break;
    case GAME_STATE_PLAYING:
        gameplayAction = gameplay_update(&game->gameplay, deltaTime_ms);
        if(gameplayAction == GAMEPLAY_ACTION_GAME_OVER){
            game->state = GAME_STATE_GAME_OVER;
        }
        break;
    case GAME_STATE_GAME_OVER:
        gameOverAction = gameOver_update(&game->gameOver);
        if(gameOverAction == GAME_OVER_ACTION_RESUME){
            game->state = GAME_STATE_MENU;
        }
        break;
    default:
        break;
    }
}
void game_render(game_t *game){
    switch (game->state){
    case GAME_STATE_MENU:
        menu_render(&game->menu, game->renderer);
        break;
    case GAME_STATE_LAUNCHING:
        renderer_clear(game->renderer, ILI9341_WHITE);
        renderer_drawMap(game->renderer, game->mapID);
        break;
    case GAME_STATE_PLAYING:
        gameplay_render(&game->gameplay, game->renderer);
        break;
    case GAME_STATE_GAME_OVER:
        gameOver_render(&game->gameOver, game->renderer);
        break;
    default:
        break;
    }
}