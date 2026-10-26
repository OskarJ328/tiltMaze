#pragma once

#include "assetManager.h"
#include "basicTypes.h"
#include "input.h"
#include "renderer.h"
#include "ui.h"

typedef enum{
    MENU_ACTION_NONE,
    MENU_ACTION_NEW_GAME
}menuAction_t;

typedef enum{
    MENU_WINDOW_MAIN,
    MENU_WINDOW_INFO
}menuWindow_t;

typedef enum{
    MENU_MAIN_NEW_GAME,
    MENU_MAIN_INFO
}menu_mainOption_t;

typedef struct{
    input_t *input;
    assetManager_t *assets;
    
    menuWindow_t window;
    menu_mainOption_t mainOption;
    
    bool buttonGraphics_wereCreated;
    bool window_wasRendered;
    bool option_wasRendered;


    screenBackground_t background;
}menu_t;


void menu_init(menu_t *menu, input_t *input, assetManager_t *assets);
menuAction_t menu_update(menu_t *menu);
void menu_render(menu_t *menu, renderer_t *renderer);