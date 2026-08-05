#include "tiltMaze.h"
#include "basicTypes.h"
#include "input.h"
#include "map.h"


static TileID tiltMaze_getTileAtPixel(map_t *map, vector2_t pixel){
    vector2_t TilePosition;
    TilePosition.x = pixel.x / map->tileSize_pixels.width;
    TilePosition.y = pixel.y / map->tileSize_pixels.height;

    uint16_t TileIdx = TilePosition.y * map->size_tiles.width + TilePosition.x;
    uint16_t maxMapIdx = map->size_tiles.width * map->size_tiles.height - 1;
    if(TileIdx > maxMapIdx){
        return TILE_NULL;
    }
    
    return map->tileIds[TileIdx];
}

/*
@param tilePosition tile idx in map 
*/
static vector2_t tiltMaze_getTileCenterPixel(vector2_t tilePosition, Size size){
    vector2_t tileCenterPixel;
    tileCenterPixel.x = tilePosition.x * size.width + size.width / 2;
    tileCenterPixel.y = tilePosition.y * size.height + size.height / 2;
    return tileCenterPixel;
}   
/*
@param tilePosition tile idx in map 
*/
static vector2_t tiltMaze_getTileLeftTopPixel(vector2_t tilePosition, Size size){
    vector2_t tileLeftTopPixel;
    tileLeftTopPixel.x = tilePosition.x * size.width;
    tileLeftTopPixel.y = tilePosition.y * size.height;
    return tileLeftTopPixel;
}
/*
@param tilePosition tile idx in map 
*/
static vector2_t tiltMaze_getTileRightBottomPixel(vector2_t tilePosition, Size size){
    vector2_t tileRightBottomPixel;
    tileRightBottomPixel.x = tilePosition.x * size.width + size.width;
    tileRightBottomPixel.y = tilePosition.y * size.height + size.height;
    return tileRightBottomPixel;
}
/*
@param objectPosition center pixel of Sprite 
*/
static vector2_t tiltMaze_getObjectLeftTopPixel(vector2_t objectPosition, Size size){
    vector2_t objectLeftTopPixel;
    objectLeftTopPixel.x = objectPosition.x - size.width;
    objectLeftTopPixel.y = objectPosition.y - size.height;
    return objectLeftTopPixel;
}
/*
@param objectPosition center pixel of Sprite 
*/
static vector2_t tiltMaze_getObjectRigthBottomPixel(vector2_t objectPosition, Size size){
    vector2_t objectRightBottomPixel;
    objectRightBottomPixel.x = objectPosition.x - size.width;
    objectRightBottomPixel.y = objectPosition.y - size.height;
    return objectRightBottomPixel;
}

static void moveUp(){
    
}

static void tiltMaze_moveBall(){
    button_t *button = input_getButton(input_t *input, ButtonID id)
}

void tiltMaze_init(tiltMaze_t *tiltMaze, map_t *map, sprite_t *ball){
    tiltMaze->map = map;
    vector2_t ball_startingPosition = tiltMaze_getTileCenterPixel(tiltMaze->map->startingTile, map->tileSize_pixels);

    tiltMaze->ball.position.x = ball_startingPosition.x;
    tiltMaze->ball.position.y = ball_startingPosition.y;
    tiltMaze->ball.velocity.x = 0;
    tiltMaze->ball.velocity.y = 0;
    tiltMaze->ball.sprite = ball;
}





