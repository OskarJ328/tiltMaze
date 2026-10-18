#include "tile.h"
#include "sprite.h"

tile_t tiles[5] = {
    [TILE_WALL] = {.spriteId = SPRITE_WALL, .type = TILE_TYPE_WALL},
    [TILE_FLOOR] = {.spriteId = SPRITE_FLOOR, .type = TILE_TYPE_FLOOR},
    [TILE_HOLE] = {.spriteId = SPRITE_HOLE, .type = TILE_TYPE_HOLE}
};