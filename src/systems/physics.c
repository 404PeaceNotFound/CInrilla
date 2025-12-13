#include "systems.h"
#include "../config/config.h"
#include <raymath.h>
#include <math.h>
#include <stdio.h>

// Função auxiliar interna para verificar colisão de um retângulo com o mapa
bool CheckMapCollisionRec(GameMap* map, Rectangle rect) {
    if (!map->loaded) return false;
    
    // Se não tiver layer de colisão, nada colide
    if (map->collisionLayerIndex < 0 || map->collisionLayerIndex >= map->layersCount) return false;

    MapLayer* groundLayer = &map->layers[map->collisionLayerIndex];
    if (!groundLayer->data) return false;

    // Converte retângulo (Pixels) para coordenadas de Grid (Tiles)
    int leftTile = (int)(rect.x / map->tileWidth);
    int rightTile = (int)((rect.x + rect.width - 0.01f) / map->tileWidth);
    int topTile = (int)(rect.y / map->tileHeight);
    int bottomTile = (int)((rect.y + rect.height - 0.01f) / map->tileHeight);

    // Limites do mundo
    if (leftTile < 0) leftTile = 0;
    if (rightTile >= map->width) rightTile = map->width - 1;
    if (topTile < 0) topTile = 0;
    if (bottomTile >= map->height) bottomTile = map->height - 1;

    for (int y = topTile; y <= bottomTile; y++) {
        for (int x = leftTile; x <= rightTile; x++) {
            int index = y * map->width + x;
            int tileID = groundLayer->data[index];

            // Qualquer tile > 0 é parede
            if (tileID > 0) return true;
        }
    }
    return false;
}

void Physics_UpdatePlayer(Player *player, GameMap* map, float dt) {
    // 1. Definição da Hitbox do Player (Centralizada nos pés)
    float w = 30.0f; 
    float h = 40.0f; 
    
    // --- EIXO X (MOVIMENTO HORIZONTAL) ---
    
    // Sonda Horizontal
    Rectangle rectX = { 
        player->position.x - w/2, 
        player->position.y - h + 2, // Levanta 2px do chão para não raspar
        w, 
        h - 8                       // Encolhe altura
    };

    if (CheckMapCollisionRec(map, rectX)) {
        // Colidiu na Horizontal
        
        if (player->PlayerDirection > 0)
        {
             // Calcula onde começa a parede à direita
             int wallTileX = (int)((rectX.x + rectX.width) / map->tileWidth);
             // Encaixa o player logo antes da parede
             player->position.x = (wallTileX * map->tileWidth) - w/2 - 0.1f;
        } 
        else if (player->PlayerDirection < 0)
        {
             // Calcula onde termina a parede à esquerda
             int wallTileX = (int)(rectX.x / map->tileWidth);
             // Encaixa o player logo depois da parede
             player->position.x = ((wallTileX + 1) * map->tileWidth) + w/2 + 0.1f;
        }
    }

    // --- EIXO Y (GRAVIDADE E PULO) ---
    player->speed += GRAVIDADE * dt;
    float deltaY = player->speed * dt;
    player->position.y += deltaY;

    // Sonda Vertical
    Rectangle rectY = { 
        player->position.x - w/2 + 2, // Encolhe largura para não prender na parede lateral
        player->position.y - h, 
        w - 4, 
        h 
    };

    if (CheckMapCollisionRec(map, rectY))
    {
        if (player->speed > 0) 
        { 
            // CAINDO
            int footTileY = (int)(player->position.y / map->tileHeight);
            
            // Corrige posição para o pixel exato acima do tile
            player->position.y = (float)(footTileY * map->tileHeight);
            
            player->speed = 0;
            player->canJump = true; 
        } 
        else if (player->speed < 0) 
        { 
            // PULANDO -> Bateu a cabeça
            int headTileY = (int)((player->position.y - h) / map->tileHeight);
            player->position.y = (float)((headTileY + 1) * map->tileHeight) + h;
            
            player->speed = 0;
        }
    } else player->canJump = false;
}

void Physics_UpdateEnemy(Enemy *enemy, GameMap *map, float dt)
{
    if (!enemy->active) return;

    // Vertical
    float nextY = enemy->position.y + enemy->verticalSpeed * dt;
    Rectangle vertRect = { enemy->position.x, nextY - enemy->height, enemy->width, enemy->height };

    if (CheckMapCollisionRec(map, vertRect))
    {
        if (enemy->verticalSpeed >= 0) 
        {
            int footTileY = (int)(nextY / map->tileHeight);
            enemy->position.y = (float)(footTileY * map->tileHeight);
            enemy->verticalSpeed = 0.0f;
        }
    } 
    else 
    {
        enemy->position.y = nextY;
        enemy->verticalSpeed += GRAVIDADE * dt; 
    }

    // Horizontal
    float moveX = enemy->direction * enemy->speed * dt;
    float nextX = enemy->position.x + moveX;
    
    Rectangle horizRect = { nextX, enemy->position.y - enemy->height + 2, enemy->width, enemy->height - 4 };
    
    bool hitWall = CheckMapCollisionRec(map, horizRect);
    bool hitPatrolLimit = (nextX + enemy->width >= enemy->maxX) || (nextX <= enemy->minX);

    if (hitWall || hitPatrolLimit) {
        enemy->direction *= -1; 
    } else {
        enemy->position.x = nextX;
    }
    
    // Timer de animação
    enemy->timer -= dt;
    if(enemy->timer < 0)
    {
        enemy->timer = enemy->frameTime;
        enemy->frame++;
        
        int maxFrames = (enemy->state == ENEMY_STATE_IDLE) ? enemy->maxFramesIdle : enemy->maxFramesWalk;
        if(enemy->frame >= maxFrames) enemy->frame = 0;
    }
}