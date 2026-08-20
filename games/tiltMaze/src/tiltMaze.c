/*
!!! PRZYJMUJE KONWENCJE ZE PRZY PARZYSTEJ SZEROKOSCI/WYSOKOSCI OBJECTU SRODEK ZNAJDUJE SIE W PIXELU BARDZIEJ NA PRAWO/NIZEJ CZYLI O WYZSZYM INDEKSIE !!!
!!! PRZYKLADOWO: WIDTH = 8, INDEKSY = {0,1,2...7} -> SRODEK = 4, HEIGHT = 10, INDEKSY = {0,1,2...9} -> SRODEK = 5 !!!


*/

#include "tiltMaze.h"
#include "basicTypes.h"
#include "button.h"
#include "input.h"
#include "map.h"
#include "tile.h"


static TileID tiltMaze_getTileAtPixel(map_t *map, vector2_t pixel){

    if(pixel.x < 0 || pixel.y < 0){
        return TILE_NULL;
    }

    vector2_t TilePosition;
    TilePosition.x = pixel.x / map->tileSize_pixels.width;
    TilePosition.y = pixel.y / map->tileSize_pixels.height;

    if(TilePosition.x >= map->size_tiles.width || TilePosition.y >= map->size_tiles.height){
        return TILE_NULL;
    } 

    uint16_t TileIdx = TilePosition.y * map->size_tiles.width + TilePosition.x;
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
    tileRightBottomPixel.x = tilePosition.x * size.width + size.width - 1;
    tileRightBottomPixel.y = tilePosition.y * size.height + size.height - 1;
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
static vector2_t tiltMaze_getObjectRightBottomPixel(vector2_t objectPosition, Size size){
    vector2_t objectRightBottomPixel;
    objectRightBottomPixel.x = objectPosition.x + (size.width - 1) / 2;
    objectRightBottomPixel.y = objectPosition.y + (size.height - 1) / 2;
    return objectRightBottomPixel;
}

/*
@param objectPosition center pixel of Sprite 
*/
static vector2_t tiltMaze_getObjectLeftBottomPixel(vector2_t objectPosition, Size size){
    vector2_t objectLeftBottomPixel;
    objectLeftBottomPixel.x = objectPosition.x - size.width / 2;
    objectLeftBottomPixel.y = objectPosition.y + (size.height - 1) / 2;
    return objectLeftBottomPixel;
}
/*
@param objectPosition center pixel of Sprite 
*/
static vector2_t tiltMaze_getObjectRightTopPixel(vector2_t objectPosition, Size size){
    vector2_t objectRightTopPixel;
    objectRightTopPixel.x = objectPosition.x + (size.width - 1) / 2;
    objectRightTopPixel.y = objectPosition.y - size.height / 2;
    return objectRightTopPixel;
}

static void tiltMaze_moveUp(tiltMaze_t *tiltMaze){ 
    vector2_t ball_nextPosition = tiltMaze->ball.position;
    ball_nextPosition.y -= 1;
    
    vector2_t ball_nextLeftTopPixel = tiltMaze_getObjectLeftTopPixel(ball_nextPosition, tiltMaze->ball.sprite->size);
    TileID ball_nextLeftTopTile = tiltMaze_getTileAtPixel(tiltMaze->map, ball_nextLeftTopPixel);
    
    vector2_t ball_nextRightTopPixel = tiltMaze_getObjectRightTopPixel(ball_nextPosition, tiltMaze->ball.sprite->size);
    TileID ball_nextRightTopTile = tiltMaze_getTileAtPixel(tiltMaze->map, ball_nextRightTopPixel);

    if(ball_nextLeftTopTile == TILE_WALL || ball_nextRightTopTile == TILE_WALL){
        return;
    }
    else{
        tiltMaze->ball.position = ball_nextPosition;
    }
    
}

static void tiltMaze_moveDown(tiltMaze_t *tiltMaze){
    vector2_t ball_nextPosition = tiltMaze->ball.position;
    ball_nextPosition.y += 1;
    
    vector2_t ball_nextRightBottomPixel = tiltMaze_getObjectRightBottomPixel(ball_nextPosition, tiltMaze->ball.sprite->size);
    TileID ball_nextRightBottomTile = tiltMaze_getTileAtPixel(tiltMaze->map, ball_nextRightBottomPixel);

    vector2_t ball_nextLeftBottomPixel = tiltMaze_getObjectLeftBottomPixel(ball_nextPosition, tiltMaze->ball.sprite->size);
    TileID ball_nextLeftBottomTile = tiltMaze_getTileAtPixel(tiltMaze->map, ball_nextLeftBottomPixel);

    if(ball_nextRightBottomTile == TILE_WALL || ball_nextLeftBottomTile == TILE_WALL){
        return;
    }
    else{
        tiltMaze->ball.position = ball_nextPosition;
    }
}

static void tiltMaze_moveLeft(tiltMaze_t *tiltMaze){
    vector2_t ball_nextPosition = tiltMaze->ball.position;
    ball_nextPosition.x -= 1;

    vector2_t ball_nextLeftTopPixel = tiltMaze_getObjectLeftTopPixel(ball_nextPosition, tiltMaze->ball.sprite->size);
    TileID ball_nextLeftTopTile = tiltMaze_getTileAtPixel(tiltMaze->map, ball_nextLeftTopPixel);

    vector2_t ball_nextLeftBottomPixel = tiltMaze_getObjectLeftBottomPixel(ball_nextPosition, tiltMaze->ball.sprite->size);
    TileID ball_nextLeftBottomTile = tiltMaze_getTileAtPixel(tiltMaze->map, ball_nextLeftBottomPixel);
    
    if(ball_nextLeftBottomTile == TILE_WALL || ball_nextLeftTopTile == TILE_WALL){
        return;
    }
    else{
        tiltMaze->ball.position = ball_nextPosition;
    }

}

static void tiltMaze_moveRight(tiltMaze_t *tiltMaze){
    vector2_t ball_nextPosition = tiltMaze->ball.position;
    ball_nextPosition.x += 1;
    
    vector2_t ball_nextRightBottomPixel = tiltMaze_getObjectRightBottomPixel(ball_nextPosition, tiltMaze->ball.sprite->size);
    TileID ball_nextRightBottomTile = tiltMaze_getTileAtPixel(tiltMaze->map, ball_nextRightBottomPixel);

    vector2_t ball_nextRightTopPixel = tiltMaze_getObjectRightTopPixel(ball_nextPosition, tiltMaze->ball.sprite->size);
    TileID ball_nextRightTopTile = tiltMaze_getTileAtPixel(tiltMaze->map, ball_nextRightTopPixel);

    if(ball_nextRightBottomTile == TILE_WALL || ball_nextRightTopTile == TILE_WALL){
        return;
    }
    else{
        tiltMaze->ball.position = ball_nextPosition;
    }
}

static void tiltMaze_moveBall(tiltMaze_t *tiltMaze, input_t *input){
    button_t *button;

    button = input_getButton(input, BUTTON_UP);
    if(button_wasPressed(button)){
        tiltMaze_moveUp(tiltMaze);
    }
    button = input_getButton(input, BUTTON_DOWN);
    if(button_wasPressed(button)){
        tiltMaze_moveDown(tiltMaze);
    }
    button = input_getButton(input, BUTTON_LEFT);
    if(button_wasPressed(button)){
        tiltMaze_moveLeft(tiltMaze);
    }
    button = input_getButton(input, BUTTON_RIGHT);
    if(button_wasPressed(button)){
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