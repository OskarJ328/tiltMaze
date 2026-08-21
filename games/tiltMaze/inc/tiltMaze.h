#pragma once

#include "assetManager.h"
#include "basicTypes.h"
#include "input.h"
#include "map.h"
#include "sprite.h"

typedef struct {
    vector2_t position;

    floatVector2_t floatPosition;
    floatVector2_t velocity;
    floatVector2_t acceleration;

    uint8_t radius;
}ball_t;

typedef struct{
    map_t *map;
    ball_t ball;
    input_t *input;

    assetManager_t *assets;
}tiltMaze_t;

void tiltMaze_init(tiltMaze_t *tiltMaze, input_t *input, assetManager_t *assets, MapID mapId, SpriteID spriteId);
void tiltMaze_update(tiltMaze_t *tiltMaze, uint32_t deltaTime_ms);