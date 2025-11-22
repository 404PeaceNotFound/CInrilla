#include "systems.h"
#include "../config/config.h"
#include <raymath.h>

void Physics_UpdatePlayer(Player *player, EnvItem *envItems, int envLength, float dt) {
    // Lógica extraída de UpdatePlayer no gameplay.c original
    // Nota: Input foi movido para entities.c, aqui aplicamos a física resultante
    
    bool hitObstacle = false;
    Vector2 *p = &(player->position);

    // Colisão vertical e chão
    for (int i = 0; i < envLength; i++) {
        EnvItem *ei = envItems + i;
        if (ei->blocking &&
            ei->rect.x <= p->x &&
            ei->rect.x + ei->rect.width >= p->x &&
            ei->rect.y >= p->y &&
            ei->rect.y <= p->y + player->speed * dt) 
        {
            hitObstacle = true;
            player->speed = 0.0f;
            p->y = ei->rect.y;
            break;
        }
    }

    if (!hitObstacle) {
        player->position.y += player->speed * dt;
        player->speed += GRAVIDADE * dt;
        player->canJump = false;
    } else {
        player->canJump = true;
    }
}