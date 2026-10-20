#include "menu.h"
#include "assetManager.h"
#include "basicTypes.h"
#include "button.h"
#include "fonts.h"
#include "ili9341.h"
#include "input.h"
#include "renderer.h"
#include "ui.h"
#include <string.h>

void menu_init(menu_t *menu, input_t *input){
    menu->input = input;
    menu->menuOption = MENU_OPTION_NEW_GAME;
    menu->wasScreenCleared = false;
}

menuAction_t menu_update(menu_t *menu){
    button_t *button;
    button = input_getButton(menu->input, BUTTON_RESUME);
    if(button_wasPressed(button)){
        menu->wasScreenCleared = false;
        return MENU_ACTION_NEW_GAME;
    }
    return MENU_ACTION_NONE;
}
/*
static void menu_drawOption(){
    uint16_t text_length = strlen(text);
    const font_t *font = assetManager_getFont(assets, id);



}
*/

void menu_render(menu_t *menu, renderer_t *renderer){
    if(!menu->wasScreenCleared){
        renderer_clear(renderer, ILI9341_WHITE);
        menu->wasScreenCleared = true;
    }
    rectangle_t rec;
    rec.position.x = 50;
    rec.position.y = 50;
    rec.size.width = 16 * 6;
    rec.size.height = 16 * 4;
    ui_drawOuterFrame(renderer, rec, SPRITE_WALL);
}