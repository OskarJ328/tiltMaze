#include "game.h"
#include "assetManager.h"
#include "gameOver.h"
#include "gameplay.h"
#include "input.h"
#include "map.h"
#include "renderer.h"
#include "sprite.h"

void game_init(game_t *game, input_t *input, renderer_t *renderer, assetManager_t *assets){
    game->input = input;
    game->renderer = renderer;
    game->menu.input = input;
    game->gameplay.input = input;
    game->pauseMenu.input = input;
    game->state = GAME_STATE_MENU;

    gameplay_init(&game->gameplay, input, assets, MAP_FANTASY, SPRITE_BALL);
    gameOver_init(&game->gameOver, input, assets, MAP_FANTASY);
    renderer_drawMap(renderer, MAP_FANTASY);
}

void game_update(game_t *game, uint32_t deltaTime_ms){
    gameplayAction_t gameplayAction;
    switch(game->state){
    case GAME_STATE_MENU:
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
        game->state = GAME_STATE_MENU;
        break;
    default:
        break;
    }
}
void game_render(game_t *game){
    switch (game->state){
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