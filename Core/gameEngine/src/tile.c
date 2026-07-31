#include "tile.h"
#include "sprite.h"

tile_t tiles[5] = {
    [TILE_WALL] = {.spriteId = SPRITE_WALL},
    [TILE_FLOOR] = {.spriteId = SPRITE_FLOOR}
};