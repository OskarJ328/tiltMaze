/*
!!! PRZYJMUJE KONWENCJE ZE PRZY PARZYSTEJ SZEROKOSCI/WYSOKOSCI OBJECTU SRODEK ZNAJDUJE SIE W PIXELU BARDZIEJ NA PRAWO/NIZEJ CZYLI O WYZSZYM INDEKSIE !!!
!!! PRZYKLADOWO: WIDTH = 8, INDEKSY = {0,1,2...7} -> SRODEK = 4, HEIGHT = 10, INDEKSY = {0,1,2...9} -> SRODEK = 5 !!!


*/

#include "tiltMaze.h"
#include "basicTypes.h"
#include "button.h"
#include "input.h"
#include "map.h"
#include "sprite.h"
#include "tile.h"
#include <stdint.h>


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

static rectangle_t tiltMaze_getTileRect(vector2_t tilePosition, Size tileSize){
    rectangle_t rect;

    rect.position.x = tilePosition.x * tileSize.width;
    rect.position.y = tilePosition.y * tileSize.height;

    rect.size = tileSize;

    return rect;
}

static bool tiltMaze_isBallColidingWithRect(vector2_t ballCenter, uint8_t radius, rectangle_t rect){
    vector2_t closestPoint;
    
    // jesli srodek kulki znajduje sie na lewo od lewej sciany prostokata to musi to byc najblizsza sciana pionowa.
    if(ballCenter.x < rect.position.x){
        closestPoint.x = rect.position.x;
    }
    // jesli srodek kulki znajduj sie na prawo od prawej sciany prostokata to musi to byc najblizsza sciana pionowa
    else if(ballCenter.x > (rect.position.x + rect.size.width - 1)){
        closestPoint.x = rect.position.x + rect.size.width - 1;
    }
    // jesli srodek kulki nie znajduje sie ani na lewo od lewej sciany ani na prawo od prawej to znaczy ze znajduje sie w osi x wewnatrz szerokosci tego prostokata
    else{
        closestPoint.x = ballCenter.x;
    }

    //analogicznie dla wysokosci
    if(ballCenter.y < rect.position.y){
        closestPoint.y = rect.position.y;
    }
    else if(ballCenter.y > rect.position.y + rect.size.height - 1){
        closestPoint.y = rect.position.y + rect.size.height - 1;
    }
    else{
        closestPoint.y = ballCenter.y;
    }

    int32_t dx = ballCenter.x - closestPoint.x;
    int32_t dy = ballCenter.y - closestPoint.y;
    
    int32_t distanceSquared = dx *dx + dy * dy;
    int32_t radiusSquared = radius * radius;

    return distanceSquared <= radiusSquared;
}

static bool tiltMaze_checkBallCollision(tiltMaze_t *tiltMaze, vector2_t nextPosition){
    uint8_t radius = tiltMaze->ball.radius;

    int32_t left    = nextPosition.x - radius;
    int32_t right   = nextPosition.x + (radius * 2 - 1) / 2;
    int32_t top      = nextPosition.y - radius;
    int32_t bottom    = nextPosition.y + (radius * 2 - 1) / 2;
    
    Size tileSize = tiltMaze->map->tileSize_pixels;

    vector2_t firstTile;
    firstTile.x = left / tileSize.width;
    firstTile.y = top /  tileSize.height;

    vector2_t lastTile;
    lastTile.x = right / tileSize.width;
    lastTile.y = bottom / tileSize.height;

    for(int16_t y = firstTile.y; y <= lastTile.y; y++){
        for(int16_t x = firstTile.x; x <= lastTile.x; x++){
            if(x < 0 || x >= tiltMaze->map->size_tiles.width){
                continue;
            }
            if(y < 0 || y >= tiltMaze->map->size_tiles.height){
                continue;
            }
            uint16_t tileIdx = y * tiltMaze->map->size_tiles.width + x;
            TileID tile = tiltMaze->map->tileIds[tileIdx];

            if(tile != TILE_WALL){
                continue;
            }
            vector2_t tilePosition = {x, y};
            rectangle_t wallRect = tiltMaze_getTileRect(tilePosition, tileSize); 
            if(tiltMaze_isBallColidingWithRect(nextPosition, radius, wallRect)){
                return true;
            }
        }
    }
    return false;
}

static void tiltMaze_moveUp(tiltMaze_t *tiltMaze){ 
    vector2_t ball_nextPosition = tiltMaze->ball.position;
    ball_nextPosition.y -= 1;
    
    if(tiltMaze_checkBallCollision(tiltMaze, ball_nextPosition)){
       return; 
    }
    tiltMaze->ball.position = ball_nextPosition;  
}

static void tiltMaze_moveDown(tiltMaze_t *tiltMaze){
    vector2_t ball_nextPosition = tiltMaze->ball.position;
    ball_nextPosition.y += 1;
    
    if(tiltMaze_checkBallCollision(tiltMaze, ball_nextPosition)){
       return; 
    }
    tiltMaze->ball.position = ball_nextPosition;
}

static void tiltMaze_moveLeft(tiltMaze_t *tiltMaze){
    vector2_t ball_nextPosition = tiltMaze->ball.position;
    ball_nextPosition.x -= 1;

    if(tiltMaze_checkBallCollision(tiltMaze, ball_nextPosition)){
       return; 
    }
    tiltMaze->ball.position = ball_nextPosition;
}

static void tiltMaze_moveRight(tiltMaze_t *tiltMaze){
    vector2_t ball_nextPosition = tiltMaze->ball.position;
    ball_nextPosition.x += 1;
    
    if(tiltMaze_checkBallCollision(tiltMaze, ball_nextPosition)){
       return; 
    }
    tiltMaze->ball.position = ball_nextPosition;
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

void tiltMaze_init(tiltMaze_t *tiltMaze, input_t *input, assetManager_t *assets, MapID mapId, SpriteID spriteId){
    tiltMaze->assets = assets;
    tiltMaze->input = input;

    map_t *map = assetManager_getMap(assets, mapId);
    tiltMaze->map = map;
    
    const sprite_t *sprite = assetManager_getSprite(assets, spriteId);
    tiltMaze->ball.radius = (uint8_t)sprite->size.width;

    vector2_t ball_startingPosition = tiltMaze_getTileCenterPixel(map->startingTile, map->tileSize_pixels);
    tiltMaze->ball.position.x = ball_startingPosition.x;
    tiltMaze->ball.position.y = ball_startingPosition.y;

    tiltMaze->ball.velocity.x = 0;
    tiltMaze->ball.velocity.y = 0;
}

void tiltMaze_update(tiltMaze_t *tiltMaze, input_t *input){
    tiltMaze_moveBall(tiltMaze, input);
}
