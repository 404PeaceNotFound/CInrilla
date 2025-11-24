#include "enemy_system.h"
#include <raylib.h>  // se precisar (por causa de Vector2 / Color / etc.)
#include <string.h>

void InitEnemy(Enemy *e, Vector2 pos, float leftLimit, float rightLimit, float speed){

    e->position = pos;
    e->verticalSpeed = 0.0f;

    e->leftLimit = leftLimit;
    e->rightLimit = rightLimit;
    e->speed = speed;
    e->direction = 1;

    e->width = 30;
    e->height = 30;

    e->frame = 0;
    e->frameTime = 0.12f;
    e->frameTimer = e->frameTime;

    e->animBackwards = 0;

    e->frameWidth = 64;
    e->frameHeight = 64;

    e->maxFramesIdle = 4;
    e->maxFramesWalk = 6;
    e->maxFramesRun = 6;
    e->maxFramesAttack = 2;

    e->rowIdle = 0;
    e->rowWalk = 1;
    e->rowRun = 2;
    e->rowAttack = 3;

    e->active = 1;
    e->state = ENEMY_STATE_WALK;

    e->useTexture = 0;
}
Enemy Enemy_Create(Vector2 pos, float leftLimit, float rightLimit, float walkSpeed){
    Enemy e;
    InitEnemy(&e, pos, leftLimit, rightLimit, walkSpeed);   
    return e;
}

void UpdateEnemy(Enemy *e, float dt){
    if (!e->active) return;

    //Movimentação fisica 
    e->position.x += e->direction * e->speed * dt;

    if (e->position.x + e->width >= e->rightLimit) {
        e->position.x = e->rightLimit - e->width;
        e->direction = -1; //-1
    }else if(e->position.x <= e->leftLimit){
        e->position.x = e->leftLimit;
        e->direction = 1; //1
    }

    e->state = ENEMY_STATE_WALK;

    int maxFrames = (e->state == ENEMY_STATE_IDLE) ? e->maxFramesIdle : e->maxFramesWalk;

    if(maxFrames <=0){
        maxFrames = 1;
    }

    e->frameTimer-= dt;

    if(e->frameTimer < 0){
        e->frameTimer = e->frameTime;
        
        e->frame++;
        if(e->frame >= maxFrames){
            e->frame = 0;
        }
    }
}
