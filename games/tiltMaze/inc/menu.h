#pragma once

#include "assetManager.h"
#include "basicTypes.h"
#include "input.h"
#include "renderer.h"

typedef enum{
    MENU_ACTION_NONE,
    MENU_ACTION_NEW_GAME
}menuAction_t;

typedef enum{
    MENU_OPTION_NEW_GAME,
    MENU_OPTION_INFO
}menuOption_t;


typedef struct{
    input_t *input;
    assetManager_t *assets;
    
    menuOption_t menuOption;
    bool wasScreenCleared;
}menu_t;


void menu_init(menu_t *menu, input_t *input);
menuAction_t menu_update(menu_t *menu);
void menu_render(menu_t *menu, renderer_t *renderer);