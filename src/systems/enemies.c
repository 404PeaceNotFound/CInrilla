#include "enemies.h"
#include <raylib.h>   // se precisar (por causa de Vector2 / Color / etc.)

void InitEnemy(Enemy *e, Vector2 pos, float leftLimit, float rightLimit, float speed)
{
    e->position = pos;
    e->leftLimit = leftLimit;
    e->rightLimit = rightLimit;
    e->speed = speed;
    e->direction = 1;
    e->width = 30;
    e->height = 60;
    e->active = 1;
    e->state = ENEMY_STATE_WALK;
}

Enemy Enemy_Create(Vector2 pos, float leftLimit, float rightLimit, float walkSpeed)
{
    Enemy e = {0};

    e.position   = pos;
    e.leftLimit  = leftLimit;
    e.rightLimit = rightLimit;
    e.speed      = walkSpeed;
    e.direction  = 1;
    e.active     = 1;

    e.frame = 0;
    e.frameTime  = 0.1f;
    e.frameTimer = 0.0f;

    e.frameWidth  = 64;
    e.frameHeight = 64;
    e.width  = e.frameWidth;
    e.height = e.frameHeight;

    e.maxFramesIdle   = 4;
    e.maxFramesWalk   = 6;
    e.maxFramesRun    = 6;
    e.maxFramesAttack = 2;

    e.rowIdle   = 0;
    e.rowWalk   = 1;
    e.rowRun    = 2;
    e.rowAttack = 3;

    e.state = ENEMY_STATE_WALK;

    return e;
}

void UpdateEnemy(Enemy *e, float dt)
{
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

    // falta a animação dos frames
}

void DrawEnemy(Enemy *e)
{
    if (!e->active) return;

    Rectangle rect = { e->position.x, e->position.y - e->height, e->width, e->height };
    DrawRectangleRec(rect, BLUE);
}
