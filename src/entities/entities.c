#include "entities.h"
#include "../config/config.h"
#include <raylib.h>

void Entities_ProcessPlayerInput(Player *player, float dt) {
    if (IsKeyDown(KEY_LEFT)) player->position.x -= PLAYER_HOR_SPD * dt;
    if (IsKeyDown(KEY_RIGHT)) player->position.x += PLAYER_HOR_SPD * dt;
    
    if (IsKeyDown(KEY_SPACE) && player->canJump) {
        player->speed = -PLAYER_JUMP_SPD;
        player->canJump = false;
    }
}