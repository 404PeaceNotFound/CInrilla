#include "entities.h"
#include "../config/config.h"
#include <raylib.h>

void Entities_ProcessPlayerInput(Player *player, float dt) {

    if(player->canJump){
        player->state = PlayerIdle;
    }
    else{
        player->state = PlayerJump;
    }

    if(player->isatk){
        player->state = PlayerAtk;
    }

    if (IsKeyDown(KEY_LEFT)) {
        player->position.x -= PLAYER_HOR_SPD * dt;
        player->PlayerDirection = -1;
        player->isatk = false;
        if(player->canJump){
            player->state = PlayerRun;
        }
    }
    if (IsKeyDown(KEY_RIGHT)){
        player->PlayerDirection = -1;
        player->position.x += PLAYER_HOR_SPD * dt;
        player->isatk = false;
        if(player->canJump){
            player->state = PlayerRun;
        }
    } 
    
    if (IsKeyDown(KEY_SPACE) && player->canJump) {
        player->speed = -PLAYER_JUMP_SPD;
        player->canJump = false;
        player->state = PlayerJump;
    }

    if (IsKeyDown(KEY_Z) && player->canJump) {
        player->isatk = true;
    }
}