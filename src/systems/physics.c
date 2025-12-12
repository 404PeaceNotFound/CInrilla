#include "systems.h"
#include "../config/config.h"
#include <raymath.h>
#include <math.h>

// Função Auxiliar
bool CheckMapCollision(GameMap* map, Rectangle rect) {
    if (!map->loaded) return false;
    
    if (map->collisionLayerIndex < 0 || map->collisionLayerIndex >= map->layersCount) {
        return false; 
    }

    MapLayer* groundLayer = &map->layers[map->collisionLayerIndex];
    if (!groundLayer->data) return false;

    float margin = 0.1f;

    int leftTile = (int)((rect.x + margin) / map->tileWidth);
    int rightTile = (int)((rect.x + rect.width - margin) / map->tileWidth);
    int topTile = (int)((rect.y + margin) / map->tileHeight);
    int bottomTile = (int)((rect.y + rect.height - margin) / map->tileHeight);

    if (leftTile < 0) leftTile = 0;
    if (rightTile >= map->width) rightTile = map->width - 1;
    if (topTile < 0) topTile = 0;
    if (bottomTile >= map->height) bottomTile = map->height - 1;

    for (int y = topTile; y <= bottomTile; y++) {
        for (int x = leftTile; x <= rightTile; x++) {
            int index = y * map->width + x;
            int tileID = groundLayer->data[index];
            if (tileID > 0) return true;
        }
    }
    return false;
}

void Physics_UpdatePlayer(Player *player, GameMap* map, float dt) {
    Vector2 oldPos = player->position;

    // Eixo X
    Rectangle rectX = { player->position.x - 20, oldPos.y - 40, 40, 40 };
    if (CheckMapCollision(map, rectX)) {
        player->position.x = oldPos.x; 
    }

    // Eixo Y
    player->speed += GRAVIDADE * dt;
    player->position.y += player->speed * dt;

    Rectangle rectY = { player->position.x - 20, player->position.y - 40, 40, 40 };
    
    if (CheckMapCollision(map, rectY)) {
        if (player->speed > 0) { // Caindo
            int footTileY = (int)((rectY.y + rectY.height) / map->tileHeight);
            player->position.y = (float)((footTileY) * map->tileHeight);
            player->speed = 0;
            player->canJump = true;
        } else if (player->speed < 0) { // Batendo cabeça
             player->speed = 0;
        }
    } else {
        player->canJump = false;
    }
}

void Physics_UpdateEnemy(Enemy *enemy, GameMap *map, float dt) {
    if (!enemy->active) return;

    // 1. Vertical
    bool hitGround = false;
    float nextY = enemy->position.y + enemy->verticalSpeed * dt;
    Rectangle vertRect = { enemy->position.x, nextY - enemy->height, enemy->width, enemy->height };

    if (CheckMapCollision(map, vertRect)) {
        if (enemy->verticalSpeed >= 0) {
            int footTileY = (int)(nextY / map->tileHeight);
            enemy->position.y = (float)(footTileY * map->tileHeight);
            enemy->verticalSpeed = 0.0f;
            hitGround = true;
        } else {
            enemy->verticalSpeed = 0.0f;
        }
    } else {
        enemy->position.y = nextY;
    }

    if (!hitGround) enemy->verticalSpeed += GRAVIDADE * dt;

    // 2. Horizontal (Patrulha)
    float moveX = enemy->direction * enemy->speed * dt;
    float nextX = enemy->position.x + moveX;
    
    Rectangle horizRect = { nextX, enemy->position.y - enemy->height + 5, enemy->width, enemy->height - 10 };
    bool hitWall = CheckMapCollision(map, horizRect);
    bool hitPatrolLimit = (nextX + enemy->width >= enemy->maxX) || (nextX <= enemy->minX);

    if (hitWall || hitPatrolLimit) {
        enemy->direction *= -1; 
        if(nextX <= enemy->minX) enemy->position.x = enemy->minX;
        else if(nextX + enemy->width >= enemy->maxX) enemy->position.x = enemy->maxX - enemy->width;
    } else {
        enemy->position.x = nextX;
    }
    
    enemy->state = ENEMY_STATE_WALK;

    // 3. Animação
    int maxFramesCurrent = 1;
    switch(enemy->state) {
        case ENEMY_STATE_IDLE: maxFramesCurrent = enemy->maxFramesIdle; break;
        case ENEMY_STATE_WALK: maxFramesCurrent = enemy->maxFramesWalk; break;
        case ENEMY_STATE_RUN:  maxFramesCurrent = enemy->maxFramesRun; break;
        case ENEMY_STATE_ATTACK: maxFramesCurrent = enemy->maxFramesAttack; break;
        default: maxFramesCurrent = 1; break;
    }
    if(maxFramesCurrent <= 0) maxFramesCurrent = 1;

    enemy->timer -= dt;
    if(enemy->timer < 0) {
        enemy->timer = enemy->frameTime;
        enemy->frame++;
        if(enemy->frame >= maxFramesCurrent) enemy->frame = 0;
    }
}