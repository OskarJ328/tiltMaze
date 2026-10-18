/*
!!! PRZYJMUJE KONWENCJE ZE PRZY PARZYSTEJ SZEROKOSCI/WYSOKOSCI OBJECTU SRODEK ZNAJDUJE SIE W PIXELU BARDZIEJ NA PRAWO/NIZEJ CZYLI O WYZSZYM INDEKSIE !!!
!!! PRZYKLADOWO: WIDTH = 8, INDEKSY = {0,1,2...7} -> SRODEK = 4, HEIGHT = 10, INDEKSY = {0,1,2...9} -> SRODEK = 5 !!!


*/

#include "renderer.h"
#define MAX_VELOCITY 10
#define MIN_VELOCITY -10

#include "gameplay.h"

static TileID gameplay_getTileAtPixel(map_t *map, vector2_t pixel){

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
static vector2_t gameplay_getTileCenterPixel(vector2_t tilePosition, Size size){
    vector2_t tileCenterPixel;
    tileCenterPixel.x = tilePosition.x * size.width + size.width / 2;
    tileCenterPixel.y = tilePosition.y * size.height + size.height / 2;
    return tileCenterPixel;
}   
/*
@param tilePosition tile idx in map 
*/
static vector2_t gameplay_getTileLeftTopPixel(vector2_t tilePosition, Size size){
    vector2_t tileLeftTopPixel;
    tileLeftTopPixel.x = tilePosition.x * size.width;
    tileLeftTopPixel.y = tilePosition.y * size.height;
    return tileLeftTopPixel;
}
/*
@param tilePosition tile idx in map 
*/
static vector2_t gameplay_getTileRightBottomPixel(vector2_t tilePosition, Size size){
    vector2_t tileRightBottomPixel;
    tileRightBottomPixel.x = tilePosition.x * size.width + size.width - 1;
    tileRightBottomPixel.y = tilePosition.y * size.height + size.height - 1;
    return tileRightBottomPixel;
}
/*
@param objectPosition center pixel of Sprite 
*/
static vector2_t gameplay_getObjectLeftTopPixel(vector2_t objectPosition, Size size){
    vector2_t objectLeftTopPixel;
    objectLeftTopPixel.x = objectPosition.x - size.width / 2;
    objectLeftTopPixel.y = objectPosition.y - size.height / 2;
    return objectLeftTopPixel;
}
/*
@param objectPosition center pixel of Sprite 
*/
static vector2_t gameplay_getObjectRightBottomPixel(vector2_t objectPosition, Size size){
    vector2_t objectRightBottomPixel;
    objectRightBottomPixel.x = objectPosition.x + (size.width - 1) / 2;
    objectRightBottomPixel.y = objectPosition.y + (size.height - 1) / 2;
    return objectRightBottomPixel;
}

/*
@param objectPosition center pixel of Sprite 
*/
static vector2_t gameplay_getObjectLeftBottomPixel(vector2_t objectPosition, Size size){
    vector2_t objectLeftBottomPixel;
    objectLeftBottomPixel.x = objectPosition.x - size.width / 2;
    objectLeftBottomPixel.y = objectPosition.y + (size.height - 1) / 2;
    return objectLeftBottomPixel;
}
/*
@param objectPosition center pixel of Sprite 
*/
static vector2_t gameplay_getObjectRightTopPixel(vector2_t objectPosition, Size size){
    vector2_t objectRightTopPixel;
    objectRightTopPixel.x = objectPosition.x + (size.width - 1) / 2;
    objectRightTopPixel.y = objectPosition.y - size.height / 2;
    return objectRightTopPixel;
}

static rectangle_t gameplay_getTileRect(vector2_t tilePosition, Size tileSize){
    rectangle_t rect;

    rect.position.x = tilePosition.x * tileSize.width;
    rect.position.y = tilePosition.y * tileSize.height;

    rect.size = tileSize;

    return rect;
}

static bool gameplay_isBallColidingWithRect(vector2_t ballCenter, uint8_t radius, rectangle_t rect){
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
    
    int32_t distanceSquared = dx * dx + dy * dy;
    int32_t radiusSquared = radius * radius;

    return distanceSquared <= radiusSquared;
}

static bool gameplay_checkBallCollision(gameplay_t *gameplay, vector2_t nextPosition){
    uint8_t radius = gameplay->ball.radius;

    int32_t left    = nextPosition.x - radius;
    int32_t right   = nextPosition.x + (radius * 2 - 1) / 2;
    int32_t top     = nextPosition.y - radius;
    int32_t bottom  = nextPosition.y + (radius * 2 - 1) / 2;
    
    Size tileSize = gameplay->map->tileSize_pixels;

    vector2_t firstTile;
    firstTile.x = left / tileSize.width;
    firstTile.y = top /  tileSize.height;

    vector2_t lastTile;
    lastTile.x = right / tileSize.width;
    lastTile.y = bottom / tileSize.height;

    for(int16_t y = firstTile.y; y <= lastTile.y; y++){
        for(int16_t x = firstTile.x; x <= lastTile.x; x++){
            if(x < 0 || x >= gameplay->map->size_tiles.width){
                continue;
            }
            if(y < 0 || y >= gameplay->map->size_tiles.height){
                continue;
            }
            uint16_t tileIdx = y * gameplay->map->size_tiles.width + x;
            TileType tileID = gameplay->map->tileIds[tileIdx];
            const tile_t *tile = assetManager_getTile(gameplay->assets, tileID);
            
            if(tile->type != TILE_TYPE_WALL){
                continue;
            }
            vector2_t tilePosition = {x, y};
            rectangle_t wallRect = gameplay_getTileRect(tilePosition, tileSize); 
            if(gameplay_isBallColidingWithRect(nextPosition, radius, wallRect)){
                return true;
            }
        }
    }
    return false;
}

static bool gameplay_isBallInHole(gameplay_t *gameplay, vector2_t ballPosition){
    TileID tileID = gameplay_getTileAtPixel(gameplay->map, ballPosition);
    const tile_t *tile = assetManager_getTile(gameplay->assets, tileID);
    if(tile->type == TILE_TYPE_HOLE){
        return true;
    }
    return false;
}

static floatVector2_t gameplay_countBallPosition(gameplay_t *gameplay, uint32_t deltaTime_ms){
    float dt_s = deltaTime_ms / 1000.0f;
    
    floatVector2_t newVelocity = gameplay->ball.velocity;
    newVelocity.x += gameplay->ball.acceleration.x * dt_s;
    newVelocity.y += gameplay->ball.acceleration.y * dt_s;
    if(newVelocity.x > MAX_VELOCITY){
        newVelocity.x = MAX_VELOCITY;
    }
    else if(newVelocity.x < MIN_VELOCITY){
        newVelocity.x = MIN_VELOCITY;
    }
    gameplay->ball.velocity.x = newVelocity.x;

    if(newVelocity.y > MAX_VELOCITY){
        newVelocity.y = MAX_VELOCITY;
    }
    else if(newVelocity.y < MIN_VELOCITY){
        newVelocity.y = MIN_VELOCITY;
    }
    gameplay->ball.velocity.y = newVelocity.y;

    floatVector2_t newFloatPosition = gameplay->ball.floatPosition;
    newFloatPosition.x += gameplay->ball.velocity.x * dt_s;
    newFloatPosition.y += gameplay->ball.velocity.y * dt_s;

    return newFloatPosition;
}

    void gameplay_setBallAtStartingPosition(ball_t *ball, map_t *map){
    vector2_t ball_startingPosition = gameplay_getTileCenterPixel(map->startingTile, map->tileSize_pixels);
    ball->position.x = ball_startingPosition.x;
    ball->position.y = ball_startingPosition.y;

    ball->floatPosition.x = ball_startingPosition.x;
    ball->floatPosition.y = ball_startingPosition.y;
    ball->velocity.x = 0.0f;
    ball->velocity.y = 0.0f;
    ball->acceleration.x = 0.0f;
    ball->acceleration.y = 0.0f;
}

static gameplayAction_t gameplay_moveBall(gameplay_t *gameplay, uint32_t deltaTime_ms){
    joystick_t *joystick;
    input_t *input = gameplay->input;
    gameplay->ball.acceleration.x = 0.0f;
    gameplay->ball.acceleration.y = 0.0f;

    /*
    button = input_getButton(input, BUTTON_UP);
    if(button_isPressed(button)){
        gameplay->ball.acceleration.y = -60.0f;
    }
    button = input_getButton(input, BUTTON_DOWN);
    if(button_isPressed(button)){
        gameplay->ball.acceleration.y = 60.0f;
    }
    button = input_getButton(input, BUTTON_LEFT);
    if(button_isPressed(button)){
        gameplay->ball.acceleration.x = -60.0f;
    }
    button = input_getButton(input, BUTTON_RIGHT);
    if(button_isPressed(button)){
        gameplay->ball.acceleration.x = 60.0f;
    }
        */
    joystick = input_getJoystick(input, LEFT_ANALOG);
    
    gameplay->ball.acceleration.x = joystick_getPosition_x(joystick) / 10;
    gameplay->ball.acceleration.y = joystick_getPosition_y(joystick) / 10;

    

    floatVector2_t newFloatPosition = gameplay_countBallPosition(gameplay, deltaTime_ms);
    vector2_t newPosition;
    if(newFloatPosition.x > 0.0f){
        newPosition.x = (int16_t)(newFloatPosition.x + 0.5f);
    }
    else{
        newPosition.x = (int16_t)(newFloatPosition.x - 0.5f);
    }

    if(newFloatPosition.y > 0.0f){
        newPosition.y = (int16_t)(newFloatPosition.y + 0.5f);
    }
    else{
        newPosition.y = (int16_t)(newFloatPosition.y - 0.5f);
    }
 
    
    if(gameplay_checkBallCollision(gameplay, newPosition)){
        gameplay->ball.velocity.x = 0.0f;
        gameplay->ball.velocity.y = 0.0f;
        return GAMEPLAY_ACTION_NONE; 
    }
    if(gameplay_isBallInHole(gameplay, newPosition)){
        //gameplay_setBallAtStartingPosition(&gameplay->ball, gameplay->map);
        return GAMEPLAY_ACTION_GAME_OVER;
    }
    gameplay->ball.position = newPosition;
    gameplay->ball.floatPosition = newFloatPosition;
    return GAMEPLAY_ACTION_NONE;  
}

void gameplay_init(gameplay_t *gameplay, input_t *input, assetManager_t *assets, MapID mapId, SpriteID spriteId){
    gameplay->assets = assets;
    gameplay->input = input;

    map_t *map = assetManager_getMap(assets, mapId);
    gameplay->map = map;
    
    const sprite_t *sprite = assetManager_getSprite(assets, spriteId);
    gameplay->ball.radius = (uint8_t)(sprite->size.width / 2);
    gameplay->ball.id = spriteId;

    gameplay_setBallAtStartingPosition(&gameplay->ball, gameplay->map);
}

void gameplay_start(gameplay_t *gameplay){
    gameplay_setBallAtStartingPosition(&gameplay->ball, gameplay->map);
}

gameplayAction_t gameplay_update(gameplay_t *gameplay, uint32_t deltaTime_ms){
    return gameplay_moveBall(gameplay, deltaTime_ms);
}

void gameplay_render(gameplay_t *gameplay, renderer_t *renderer){
    renderer_drawMovingObject(renderer, gameplay->ball.id, gameplay->map->id, gameplay->ball.position);
}
