#include "tiltMaze.h"
#include "basicTypes.h"
#include "button.h"
#include "input.h"
#include "map.h"
#include "tile.h"


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
    objectLeftTopPixel.x = objectPosition.x - size.width / 2;
    objectLeftTopPixel.y = objectPosition.y - size.height / 2;
    return objectLeftTopPixel;
}
/*
@param objectPosition center pixel of Sprite 
*/
static vector2_t tiltMaze_getObjectRigthBottomPixel(vector2_t objectPosition, Size size){
    vector2_t objectRightBottomPixel;
    objectRightBottomPixel.x = objectPosition.x + size.width / 2;
    objectRightBottomPixel.y = objectPosition.y + size.height / 2;
    return objectRightBottomPixel;
}

static void tiltMaze_moveUp(tiltMaze_t *tiltMaze){
    
    vector2_t ball_nextPosition = tiltMaze->ball.position;
    ball_nextPosition.y -= 1;
    vector2_t ball_nextLeftTopPixel = tiltMaze_getObjectLeftTopPixel(ball_nextPosition, tiltMaze->ball.sprite->size);
    TileID ball_leftTopTile = tiltMaze_getTileAtPixel(tiltMaze->map, ball_nextLeftTopPixel);

    if(ball_leftTopTile == TILE_WALL){
        return;
    }
    else{
        tiltMaze->ball.position = ball_nextPosition;
    }
}

static void tiltMaze_moveDown(tiltMaze_t *tiltMaze){

}

static void tiltMaze_moveLeft(tiltMaze_t *tiltMaze){

}

static void tiltMaze_moveRight(tiltMaze_t *tiltMaze){

}

static void tiltMaze_moveBall(tiltMaze_t *tiltMaze, input_t *input){
    button_t *button;

    button = input_getButton(input, BUTTON_UP);
    if(button_isPressed(button)){
        tiltMaze_moveUp(tiltMaze);
    }
    button = input_getButton(input, BUTTON_DOWN);
    if(button_isPressed(button)){
        tiltMaze_moveDown(tiltMaze);
    }
    button = input_getButton(input, BUTTON_LEFT);
    if(button_isPressed(button)){
        tiltMaze_moveLeft(tiltMaze);
    }
    button = input_getButton(input, BUTTON_RIGHT);
    if(button_isPressed(button)){
        tiltMaze_moveRight(tiltMaze);
    }
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





