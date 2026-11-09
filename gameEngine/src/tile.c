#include "tile.h"
#include "sprite.h"

tile_t tiles[10] = {
    [TILE_WALL_1] = {.spriteId = SPRITE_WALL_1, .type = TILE_TYPE_WALL},
    [TILE_WALL_2] = {.spriteId = SPRITE_WALL_2, .type = TILE_TYPE_WALL},
    [TILE_WALL_3] = {.spriteId = SPRITE_WALL_3, .type = TILE_TYPE_WALL},
    [TILE_FLOOR_1] = {.spriteId = SPRITE_FLOOR_1, .type = TILE_TYPE_FLOOR},
    [TILE_FLOOR_2] = {.spriteId = SPRITE_FLOOR_2, .type = TILE_TYPE_FLOOR},
    [TILE_FLOOR_3] = {.spriteId = SPRITE_FLOOR_3, .type = TILE_TYPE_FLOOR},
    [TILE_HOLE] = {.spriteId = SPRITE_HOLE, .type = TILE_TYPE_HOLE}
};