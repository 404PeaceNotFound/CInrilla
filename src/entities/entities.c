#include "entities.h"
#include "../systems/systems.h"
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
        if(IsSoundValid(player->soundPlayer.Jump)) PlaySound(player->soundPlayer.Jump);
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
        
        if(IsSoundValid(player->soundPlayer.Atk)) PlaySound(player->soundPlayer.Atk);
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
    
    // 1. Física Básica
    e.position = pos;
    e.minX = minX; // Antigo leftLimit
    e.maxX = maxX; // Antigo rightLimit
    e.speed = speed;
    e.verticalSpeed = 0.0f;
    e.direction = 1;
    e.active = true;
    e.health = 3; 
    
    // 2. Dimensões Padrão
    e.width = 30;
    e.height = 30;

    // 3. Estado Inicial
    e.state = ENEMY_STATE_WALK; 

    // 4. Configuração de Animação Padrão (Fallback)
    // Esses valores evitam crash se Render_ConfigEnemy não for chamado,
    // mas idealmente serão sobrescritos por ele.
    e.frame = 0;
    e.frameTime = 0.12f;
    e.timer = e.frameTime; // Antigo frameTimer
    
    e.frameWidth = 64;
    e.frameHeight = 64;
    
    e.maxFramesIdle = 4;
    e.maxFramesWalk = 6;
    e.maxFramesRun = 6;
    e.maxFramesAttack = 2;

    e.rowIdle = 0;
    e.rowWalk = 1;
    e.rowRun = 2;
    e.rowAttack = 3;

    e.useTexture = false; // Começa sem textura até carregar assets
    
    return e;
}