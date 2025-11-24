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
        player->PlayerDirection = 1;
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
        PlaySound(player->soundPlayer.Jump);
        
    }

    if (IsKeyDown(KEY_Z) && player->canJump) {
        player->isatk = true;
        player->state = PlayerAtk;
        player->anim[PlayerAtk].indiceFrameX = 0;
        player->anim[PlayerAtk].final = false;
        player->anim[PlayerAtk].temporizador = player->anim[PlayerAtk].tempoPorFrame;

        PlaySound(player->soundPlayer.Atk);
    }
    

    //Controle de Audio
    if(player->state == PlayerRun){
        if (!IsSoundPlaying(player->soundPlayer.Run)) {
            PlaySound(player->soundPlayer.Run);
        }
    }
    else{
        if (IsSoundPlaying(player->soundPlayer.Run)) {
            StopSound(player->soundPlayer.Run);
    }
}

}