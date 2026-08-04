#include "tiltMaze.h"


static TileID tiltMaze_getTileAtPixel(tiltMaze_t *tiltMaze, vector2_t position){
    vector2_t TilePosition;
    TilePosition.x = position.x / tiltMaze->map->tileSize_pixels;
    TilePosition.y = position.y / tiltMaze->map->tileSize_pixels;

    uint16_t TileIdx = TilePosition.y * tiltMaze->map->size_tiles.width + TilePosition.x;
    uint16_t maxMapIdx = tiltMaze->map->size_tiles.width * tiltMaze->map->size_tiles.height - 1;
    if(TileIdx > maxMapIdx){
        return TILE_NULL;
    }
    
    return tiltMaze->map->tileIds[TileIdx];
}

static void 


void tiltMaze_init(tiltMaze_t *tiltMaze, map_t *map, vector2_t ball_startingPosition){
    tiltMaze->map = map;

    tiltMaze->ball.position.x = ball_startingPosition.x;
    tiltMaze->ball.position.y = ball_startingPosition.y;
    tiltMaze->ball.velocity.x = 0;
    tiltMaze->ball.velocity.y = 0;
}

