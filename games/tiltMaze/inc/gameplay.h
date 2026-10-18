#pragma once

#include "assetManager.h"
#include "basicTypes.h"
#include "input.h"
#include "map.h"
#include "renderer.h"
#include "sprite.h"

typedef struct {
    vector2_t position;

    floatVector2_t floatPosition;
    floatVector2_t velocity;
    floatVector2_t acceleration;

    uint8_t radius;

    SpriteID id;
}ball_t;

typedef enum{
    GAMEPLAY_ACTION_NONE,
    GAMEPLAY_ACTION_PAUSE,
    GAMEPLAY_ACTION_LEVEL_COMPLETED,
    GAMEPLAY_ACTION_GAME_OVER
}gameplayAction_t;

typedef struct{
    map_t *map;
    ball_t ball;

    input_t *input;
    assetManager_t *assets;
}gameplay_t;




void gameplay_init(gameplay_t *gameplay, input_t *input, assetManager_t *assets, MapID mapId, SpriteID spriteId);
void gameplay_start(gameplay_t *gameplay);
gameplayAction_t gameplay_update(gameplay_t *gameplay, uint32_t deltaTime_ms);
void gameplay_render(gameplay_t *gameplay, renderer_t *renderer);