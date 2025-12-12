#include "entities.h" // Se tiver um header próprio, ou incluir systems.h
#include "../data/entity_types.h"
#include <raylib.h>

// Input do Player
void Entities_ProcessPlayerInput(Player *player, float dt) {
    // Reset estado para Idle se não estiver fazendo nada
    if (!player->isatk && player->canJump) {
        player->state = PlayerIdle;
    }

    // Movimento Lateral
    if (IsKeyDown(KEY_RIGHT)) {
        player->position.x += 300.0f * dt; // PLAYER_HOR_SPD hardcoded ou via config.h
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
        // Tocar som de pulo se existir
        if(IsSoundReady(player->soundPlayer.Jump)) PlaySound(player->soundPlayer.Jump);
    }

    // Estado de Pulo
    if (!player->canJump) {
        player->state = PlayerJump;
    }

    // Ataque
    if (IsKeyPressed(KEY_Z) && !player->isatk) {
        player->isatk = true;
        player->hasHit = false;
        player->state = PlayerAtk;
        // Reset da animação de ataque
        player->anim[PlayerAtk].indiceFrameX = 0;
        player->anim[PlayerAtk].final = false;
        
        if(IsSoundReady(player->soundPlayer.Atk)) PlaySound(player->soundPlayer.Atk);
    }

    // Controle de fim de ataque
    if (player->isatk) {
        if (player->anim[PlayerAtk].final) {
            player->isatk = false;
            player->state = PlayerIdle;
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
    e.verticalSpeed = 0;
    e.direction = 1;
    e.active = true;
    e.health = 3; // Valor padrão
    e.state = ENEMY_STATE_WALK; // Começa andando
    
    // Valores padrão de renderização (serão sobrescritos por Render_ConfigEnemy)
    e.width = 32;
    e.height = 32;
    e.useTexture = false;
    e.timer = 0;
    e.frame = 0;
    
    return e;
}