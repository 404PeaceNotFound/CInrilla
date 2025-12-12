#include "entities.h"
#include "../systems/systems.h"
#include "../data/entity_types.h"
#include <raylib.h>

// Input do Player
void Entities_ProcessPlayerInput(Player *player, float dt) {
    
    // 1. Atualiza Timer de Invencibilidade (NOVO)
    if (player->hurtTimer > 0) {
        player->hurtTimer -= dt;
    }

    // Reset estado para Idle se não estiver fazendo nada
    if (!player->isatk && player->canJump) {
        player->state = PlayerIdle;
    }

    // Movimento Lateral
    if (IsKeyDown(KEY_RIGHT)) {
        player->position.x += 300.0f * dt; 
        player->PlayerDirection = 1;
        if (!player->isatk && player->canJump) player->state = PlayerRun;
    }
    if (IsKeyDown(KEY_LEFT)) {
        player->position.x -= 300.0f * dt;
        player->PlayerDirection = -1;
        if (!player->isatk && player->canJump) player->state = PlayerRun;
    }

    // Pulo
    if (IsKeyPressed(KEY_SPACE) && player->canJump) {
        player->speed = -500.0f; // PLAYER_JUMP_SPD
        player->canJump = false;
        
        if(IsSoundValid(player->soundPlayer.Jump)) PlaySound(player->soundPlayer.Jump);
    }

    // Estado de Pulo (Só entra se não estiver atacando no chão)
    // Pequena correção lógica: Se atacar no ar, mantemos a animação de ataque ou pulo?
    // Geralmente ataque prioriza.
    if (!player->canJump && !player->isatk) {
        player->state = PlayerJump;
    }

    // Ataque
    if (IsKeyPressed(KEY_Z) && !player->isatk) {
        player->isatk = true;
        player->hasHit = false;
        player->state = PlayerAtk;
        
        player->anim[PlayerAtk].indiceFrameX = 0; // Reinicia frame
        player->anim[PlayerAtk].temporizador = 0; // Reinicia tempo
        player->anim[PlayerAtk].final = false;    // Reinicia flag de fim
        
        if(IsSoundValid(player->soundPlayer.Atk)) PlaySound(player->soundPlayer.Atk);
    }

    // Controle de fim de ataque
    if (player->isatk) {
        if (player->anim[PlayerAtk].final) {
            player->isatk = false;
            // Retorna para Idle ou Pulo dependendo da gravidade
            player->state = player->canJump ? PlayerIdle : PlayerJump;
        }
    }
}

// Factory de Inimigos
Enemy Enemy_Create(Vector2 pos, float minX, float maxX, float speed) {
    Enemy e = {0};
    
    e.position = pos;
    e.minX = minX;
    e.maxX = maxX;
    e.speed = speed;
    e.verticalSpeed = 0.0f;
    e.direction = 1;
    e.active = true;
    e.health = 3; 
    
    e.width = 30;
    e.height = 30;

    e.state = ENEMY_STATE_WALK; 

    // Padrões de segurança
    e.frame = 0;
    e.frameTime = 0.12f;
    e.timer = e.frameTime; 
    e.frameWidth = 64; e.frameHeight = 64;
    e.maxFramesIdle = 4; e.maxFramesWalk = 6; e.maxFramesRun = 6; e.maxFramesAttack = 2;
    e.rowIdle = 0; e.rowWalk = 1; e.rowRun = 2; e.rowAttack = 3;
    e.useTexture = false; 
    
    return e;
}