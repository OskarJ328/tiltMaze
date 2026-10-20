#include "gameOver.h"
#include "assetManager.h"
#include "basicTypes.h"
#include "button.h"
#include "fonts.h"
#include "ili9341.h"
#include "input.h"
#include "map.h"
#include "renderer.h"
#include "ui.h"
#include <string.h>

void gameOver_init(gameOver_t *gameOver, input_t *input, assetManager_t *assets, MapID mapID){
    gameOver->input = input;
    gameOver->assets = assets;
    map_t *map = assetManager_getMap(assets, mapID);
    gameOver->map = map;
    gameOver->wasScreenCleared = false;
}

gameOverAction_t gameOver_update(gameOver_t *gameOver){
    button_t *button = input_getButton(gameOver->input, BUTTON_RESUME);
    if(button_wasPressed(button)){
        gameOver->wasScreenCleared = false;
        return GAME_OVER_ACTION_RESUME;
    }
    return GAME_OVER_ACTION_NONE;
}

void gameOver_render(gameOver_t *gameOver, renderer_t *renderer){
    if(!gameOver->wasScreenCleared){
        renderer_clear(renderer, ILI9341_WHITE);
        gameOver->wasScreenCleared = true;
    }
    rectangle_t screen;
    screen.position.x = 0;
    screen.position.y = 0;
    screen.size.width = renderer->ili9341->width;
    screen.size.height = renderer->ili9341->height;
    
    ui_drawTextInRectangle(renderer, screen, FONT_BIG, "GAME OVER");
}