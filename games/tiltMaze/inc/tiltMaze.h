#pragma once

#include "basicTypes.h"
#include "input.h"
#include "map.h"
#include "sprite.h"

typedef struct {
    vector2_t position;
    vector2_t velocity;
    const sprite_t *sprite;
}ball_t;

typedef struct{
    map_t *map;
    ball_t ball;
}tiltMaze_t;

void tiltMaze_init(tiltMaze_t *tiltMaze, map_t *map, const sprite_t *ball);
void tiltMaze_update(tiltMaze_t *tiltMaze, input_t *input);