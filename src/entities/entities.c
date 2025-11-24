#include "entities.h"
#include "../config/config.h"
#include <raylib.h>

void Entities_ProcessPlayerInput(Player *player, float dt) {

    // ----------------------------------------------------------------
    // 1. INPUT DE ATAQUE (Prioridade Máxima)
    // ----------------------------------------------------------------
    //IsKeyPressed para não resetar a animação todo frame
    if (IsKeyPressed(KEY_Z) && !player->isatk) {
        player->isatk = true;
        player->state = PlayerAtk;
        
        // Reset manual da animação para garantir que comece do frame 0
        player->anim[PlayerAtk].indiceFrameX = 0;
        player->anim[PlayerAtk].indiceFrameY = 0;
        player->anim[PlayerAtk].temporizador = player->anim[PlayerAtk].tempoPorFrame;
        player->anim[PlayerAtk].final = false;

        PlaySound(player->soundPlayer.Atk);
    }

    // Se o jogador estiver no meio de um ataque, IGNORA movimentação (opcional)
    // Se quiser que ele ande atacando, remova este bloco 'if'.
    // Mas geralmente, travar o movimento dá mais peso ao golpe.
    if (player->isatk) {
        player->state = PlayerAtk;
        // Se a animação acabou (sinalizada pelo render.c), libera o player
        if (player->anim[PlayerAtk].final) {
            player->isatk = false;
            player->state = PlayerIdle;
        }
        // Retorna cedo para impedir que o código abaixo mude o state para Run
        return; 
    }

    // ----------------------------------------------------------------
    // 2. INPUT DE MOVIMENTO (Só roda se não estiver atacando)
    // ----------------------------------------------------------------
    
    // Variável para saber se houve movimento neste frame
    bool isMoving = false; 

    if (IsKeyDown(KEY_LEFT)) {
        player->position.x -= PLAYER_HOR_SPD * dt;
        player->PlayerDirection = -1;
        isMoving = true;
    }
    else if (IsKeyDown(KEY_RIGHT)){
        player->position.x += PLAYER_HOR_SPD * dt;
        player->PlayerDirection = 1;
        isMoving = true;
    } 

    // ----------------------------------------------------------------
    // 3. INPUT DE PULO
    // ----------------------------------------------------------------
    if (IsKeyPressed(KEY_SPACE) && player->canJump) {
        player->speed = -PLAYER_JUMP_SPD;
        player->canJump = false;
        player->state = PlayerJump;
        PlaySound(player->soundPlayer.Jump);
    }

    // ----------------------------------------------------------------
    // 4. DEFINIÇÃO DE ESTADO (Atualização Visual)
    // ----------------------------------------------------------------

    if (!player->canJump) {
        // Se não pode pular, é porque está no ar
        player->state = PlayerJump;
    } 
    else if (isMoving) {
        // No chão e movendo
        player->state = PlayerRun;
    } 
    else {
        // No chão e parado
        player->state = PlayerIdle;
    }

    // ----------------------------------------------------------------
    // 5. CONTROLE DE ÁUDIO (Passos)
    // ----------------------------------------------------------------
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