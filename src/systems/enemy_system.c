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
    e->height = 60;

    e->frame = 0;
    e->frameTime = 0.12f;
    e->frameTimer = 0;

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

    e->position.x += e->direction * e->speed * dt;

    if (e->position.x + e->width >= e->rightLimit) {
        e->position.x = e->rightLimit - e->width;
        e->direction = -1;
    }

    if (e->position.x <= e->leftLimit) {
        e->position.x = e->leftLimit;
        e->direction = 1;
    }

    // animações de frame(apenas teste)
    e->frameTimer += dt;
    if (e->frameTimer >= e->frameTime) {
        e->frameTimer = 0;

        int maxFrames =
            (e->state == ENEMY_STATE_IDLE)   ? e->maxFramesIdle :
            (e->state == ENEMY_STATE_WALK)   ? e->maxFramesWalk :
            (e->state == ENEMY_STATE_RUN)    ? e->maxFramesRun :
                                               e->maxFramesAttack;

        e->frame = (e->frame + 1) % maxFrames;
    }
}

void DrawEnemy(Enemy *e){ //(new draw)
    if (!e->active) return;

    // Se não há textura, desenha retângulo padrão
    if (!e->useTexture) {
        Rectangle r = { e->position.x, e->position.y - e->height, e->width, e->height };
        DrawRectangleRec(r, BLUE);
        return;
    }

    // Desenho com spritesheet
    int row =
        (e->state == ENEMY_STATE_IDLE)   ? e->rowIdle :
        (e->state == ENEMY_STATE_WALK)   ? e->rowWalk :
        (e->state == ENEMY_STATE_RUN)    ? e->rowRun :
                                           e->rowAttack;

    Rectangle src = {
        e->frame * e->frameWidth,
        row * e->frameHeight,
        e->frameWidth * e->direction, // flip horizontal se direção == -1
        e->frameHeight
    };  

    float spriteOffsetX = (e->frameWidth - e->width) / 2.0f;

    Rectangle dst = {
        e->position.x - spriteOffsetX, // recuar o desenhor para esquerda 
        e->position.y - e->frameHeight,
        e->frameWidth,
        e->frameHeight
    };

    DrawTexturePro(e->texture, src, dst, (Vector2){0, 0}, 0.0f, WHITE);
}
