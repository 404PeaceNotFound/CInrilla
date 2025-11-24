#include "systems.h"
#include "../config/config.h"
#include <raymath.h>
#include <math.h>

// Verifica se um retângulo colide com algum tile sólido na camada Ground
bool CheckMapCollision(GameMap* map, Rectangle rect) {
    if (!map->loaded || !map->layerGround.data) return false;

    // Converte bordas do player para coordenadas de Tile
    int leftTile = (int)(rect.x / map->tileWidth);
    int rightTile = (int)((rect.x + rect.width) / map->tileWidth);
    int topTile = (int)(rect.y / map->tileHeight);
    int bottomTile = (int)((rect.y + rect.height) / map->tileHeight);

    // Limites do mapa
    if (leftTile < 0) leftTile = 0;
    if (rightTile >= map->width) rightTile = map->width - 1;
    if (topTile < 0) topTile = 0;
    if (bottomTile >= map->height) bottomTile = map->height - 1;

    // Loop apenas nos tiles que o player está tocando
    for (int y = topTile; y <= bottomTile; y++) {
        for (int x = leftTile; x <= rightTile; x++) {
            int index = y * map->width + x;
            int tileID = map->layerGround.data[index];

            // Se tileID > 0, consideramos sólido na camada Ground
            if (tileID > 0) {
                return true;
            }
        }
    }
    return false;
}

// Resolve colisão eixo Y (simples)
float ResolveMapCollisionY(GameMap* map, Rectangle rect, float speedY) {
    if (!map->loaded) return rect.y;

    // Tenta mover
    if (CheckMapCollision(map, rect)) {
        // Se estava descendo (caindo), colidiu com chão
        if (speedY > 0) {
            // Encaixa no tile acima (Floor)
            int tileY = (int)((rect.y + rect.height) / map->tileHeight);
            return (tileY * map->tileHeight) - rect.height;
        }
        // Se estava subindo (pulo), colidiu com teto
        else if (speedY < 0) {
            int tileY = (int)(rect.y / map->tileHeight);
            return (float)((tileY + 1) * map->tileHeight);
        }
    }
    return rect.y;
}

void Physics_UpdatePlayer(Player *player, GameMap* map, float dt) {
    // Salva posição anterior para resolução
    Vector2 oldPos = player->position;

    // --- Eixo X ---
    // (A lógica de movimento X já foi processada no Input, aqui apenas validamos)
    // Criamos retângulo futuro X
    Rectangle rectX = { player->position.x - 20, oldPos.y - 40, 40, 40 };
    
    if (CheckMapCollision(map, rectX)) {
        player->position.x = oldPos.x; // Cancela movimento se bateu na parede
    }

    // --- Eixo Y ---
    player->speed += GRAVIDADE * dt;
    player->position.y += player->speed * dt;

    Rectangle rectY = { player->position.x - 20, player->position.y - 40, 40, 40 };
    
    // Verifica colisão
    if (CheckMapCollision(map, rectY)) {
        // Colisão detectada
        if (player->speed > 0) { // Caindo
             // Snap grid
             player->position.y = ResolveMapCollisionY(map, rectY, player->speed);
             player->speed = 0;
             player->canJump = true;
        } else if (player->speed < 0) { // Pulando e bateu cabeça
             player->position.y = ResolveMapCollisionY(map, rectY, player->speed);
             player->speed = 0;
        }
    } else {
        player->canJump = false;
    }
}