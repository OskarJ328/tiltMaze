#include "menu.h"
#include "assetManager.h"
#include "basicTypes.h"
#include "button.h"
#include "fonts.h"
#include "ili9341.h"
#include "input.h"
#include "joystick.h"
#include "renderer.h"
#include "sprite.h"
#include "ui.h"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define JOYSTICK_UP 80.0f
#define JOYSTICK_DOWN -80.0f
#define SPACE_BETWEEN_BUTTON_GRAPHICS_PXL 10

static buttonGraphic_t newGame_buttonGraphic;
static buttonGraphic_t settings_buttonGraphic;
static buttonGraphic_t info_buttonGraphic;

/*
LETTER_NUMBER = NUMBER OF LETTERS FROM LONGEST BUTTON GRAPHIC
RECTANGLE_WIDTH     = (LETTER_NUMBER + 2) * LETTER_WIDTH
RECTANGLE_HEIGHT    = LETTER_HEIGHT * 2
*/
static void menu_createButtonGraphics(assetManager_t *assets, Size screenSize){

    FontID fontID = FONT_GOTH;
    color_t fontColor = ILI9341_WHITE;
    SpriteID frameID = SPRITE_WALL;
    SpriteID backgroundID = SPRITE_FLOOR;
    SpriteID pointerID = SPRITE_BALL;

    const font_t *font = assetManager_getFont(assets, fontID);    
    const sprite_t *frame = assetManager_getSprite(assets, frameID);

    Size btnGrphs_size;
    btnGrphs_size.width = (8 + 2) * font->width; //NEW GAME = 8 
    btnGrphs_size.height = font->height * 2;
    
    vector2_t newGame_pos;
    newGame_pos.x = screenSize.width / 2 - ((btnGrphs_size.width + frame->size.width) / 2);
    newGame_pos.y = 48;

    rectangle_t newGame_rectangle = {newGame_pos, btnGrphs_size};

    newGame_buttonGraphic = ui_createButtonGraphic(fontID, ILI9341_WHITE, backgroundID, frameID, pointerID, newGame_rectangle);

    vector2_t settings_pos;
    settings_pos.x = newGame_pos.x;
    settings_pos.y = newGame_pos.y + btnGrphs_size.height + frame->size.height * 2 + SPACE_BETWEEN_BUTTON_GRAPHICS_PXL;

    rectangle_t settings_rectangle = {settings_pos, btnGrphs_size};

    settings_buttonGraphic = ui_createButtonGraphic(fontID, fontColor, backgroundID, frameID, pointerID, settings_rectangle);

    vector2_t info_pos;
    info_pos.x = settings_pos.x;
    info_pos.y = settings_pos.y + btnGrphs_size.height + frame->size.height * 2 + SPACE_BETWEEN_BUTTON_GRAPHICS_PXL;

    rectangle_t info_rectangle = {info_pos, btnGrphs_size};

    info_buttonGraphic = ui_createButtonGraphic(fontID, fontColor, backgroundID, frameID, pointerID, info_rectangle);
}

void menu_init(menu_t *menu, input_t *input, assetManager_t *assets){
    menu->input = input;
    menu->assets = assets;

    menu->mainOption = MENU_MAIN_NEW_GAME;
    menu->window = MENU_WINDOW_MAIN;

    menu->window_wasRendered = false;
    menu->option_wasRendered = false;
    menu->buttonGraphics_wereCreated = false;

    menu->background.centerID = SPRITE_FLOOR;
    menu->background.frameID = SPRITE_WALL;

}

menuAction_t menu_update(menu_t *menu){
    button_t *button_confirm    = input_getButton(menu->input, BUTTON_CONFIRM);
    button_t *button_back       = input_getButton(menu->input, BUTTON_BACK);
    joystick_t *joystick        = input_getJoystick(menu->input, LEFT_ANALOG);

    switch (menu->window){
    case MENU_WINDOW_MAIN:
        switch(menu->mainOption){
        case MENU_MAIN_NEW_GAME:
            if(button_wasPressed(button_confirm)){
                menu->window_wasRendered = false;
                menu->option_wasRendered = false;
                return MENU_ACTION_NEW_GAME;
            }
            if(joystick_hasCrossedThreshold_y(joystick, JOYSTICK_UP)){
                menu->option_wasRendered = false;
                menu->mainOption = MENU_MAIN_INFO;
            }
            else if(joystick_hasCrossedThreshold_y(joystick, JOYSTICK_DOWN)){
                menu->option_wasRendered = false;
                menu->mainOption = MENU_MAIN_INFO;
            }
            break;

        case MENU_MAIN_INFO:
            if(button_wasPressed(button_confirm)){
                menu->window_wasRendered = false;
                menu->option_wasRendered = false;
                menu->window = MENU_WINDOW_INFO;
                break;
            }
            if(joystick_hasCrossedThreshold_y(joystick, JOYSTICK_UP)){
                menu->option_wasRendered = false;
                menu->mainOption = MENU_MAIN_NEW_GAME;
            }
            else if(joystick_hasCrossedThreshold_y(joystick, JOYSTICK_DOWN)){
                menu->option_wasRendered = false;
                menu->mainOption = MENU_MAIN_NEW_GAME;
            }
            break;

        default:
            break;
            
        }
    case MENU_WINDOW_INFO:
        if(button_wasPressed(button_back)){
            menu->window_wasRendered = false;
            menu->option_wasRendered = false;
            menu->window = MENU_WINDOW_MAIN;
        }
        break;
    }
    return MENU_ACTION_NONE;
}
/*
static void menu_drawOption(){
    uint16_t text_length = strlen(text);
    const font_t *font = assetManager_getFont(assets, id);



}
*/

static void menu_renderMainWindow(menu_t *menu, renderer_t *renderer){
    if(!menu->buttonGraphics_wereCreated){
        Size screenSize;
        screenSize.width = renderer->ili9341->width;
        screenSize.height = renderer->ili9341->height;

        menu_createButtonGraphics(menu->assets, screenSize);

        menu->buttonGraphics_wereCreated = true;
    }

    ui_drawScreenBackground(renderer, menu->background);
    ui_drawButton(renderer, newGame_buttonGraphic, "New Game");
    ui_drawButton(renderer, settings_buttonGraphic, "Settings");
    ui_drawButton(renderer, info_buttonGraphic, "Info");

}

static void menu_renderInfoWindow(renderer_t *renderer){
    renderer_clear(renderer, ILI9341_WHITE);
    vector2_t position = {80,80};

    renderer_drawText(renderer, FONT_GOTH, "Usmiechnij sie\ni zabij mnie!", position, ILI9341_RED);
}



/*
TILT MAZE

NEW GAME
SETTINGS
INFO

*/

void menu_render(menu_t *menu, renderer_t *renderer){

    switch(menu->window){
    case MENU_WINDOW_MAIN:
        if(!menu->window_wasRendered){
            menu_renderMainWindow(menu, renderer);
            menu->window_wasRendered = true;
        }
        if(!menu->option_wasRendered){
            buttonGraphic_t chosenButtonGraphic;
            switch(menu->mainOption){
            case MENU_MAIN_NEW_GAME:
                chosenButtonGraphic = newGame_buttonGraphic;
                chosenButtonGraphic.fontColor = ILI9341_RED;
                ui_drawButton(renderer, chosenButtonGraphic, "New Game");
                ui_drawButton(renderer, info_buttonGraphic, "Info");
                //ui_clearButtonPointer(renderer, btnGrph_info);
                //ui_drawButtonPointer(renderer, btnGrph_newGame);
                break;
            case MENU_MAIN_INFO:
                chosenButtonGraphic = info_buttonGraphic;
                chosenButtonGraphic.fontColor = ILI9341_RED;
                ui_drawButton(renderer, chosenButtonGraphic, "Info");
                ui_drawButton(renderer, newGame_buttonGraphic, "New Game");
                //ui_clearButtonPointer(renderer, btnGrph_newGame);
                //ui_drawButtonPointer(renderer, btnGrph_info);
                break;
            default:
                break;
            }
            menu->option_wasRendered = true;
        }

        break;
    case MENU_WINDOW_INFO:
        if(!menu->window_wasRendered){
            menu_renderInfoWindow(renderer);
            menu->window_wasRendered = true;
        }
        break;
    default:
        break;
    }




    /*
    if(!menu->wasRendered){
        renderer_clear(renderer, ILI9341_WHITE);


        rectangle_t rec;
        rec.position.x = 50;
        rec.position.y = 50;
        rec.size.width = 16 * 14;
        rec.size.height = 16 * 2;

        buttonGraphic_t btnGrph;
        btnGrph.background = SPRITE_FLOOR;
        btnGrph.frame = SPRITE_WALL;
        btnGrph.fontColor = ILI9341_WHITE;
        btnGrph.fontID = FONT_GOTH;

        ui_drawButton(renderer, rec, btnGrph, "TILT MAZE :O");


        menu->wasRendered = true;
    }
    */
}