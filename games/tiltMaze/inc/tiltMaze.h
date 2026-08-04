#pragma once

#include "basicTypes.h"
#include "map.h"

typedef struct {
    vector2_t position;
    vector2_t velocity;
}ball_t;

typedef struct{
    map_t *map;
    ball_t ball;
}tiltMaze_t;

void tiltMaze_init(tiltMaze_t *tiltMaze, map_t *map, vector2_t ball_startingPosition);