#include "entities.h"
#include "../systems/systems.h"
#include "../data/entity_types.h"
#include <raylib.h>
#include "../config/config.h"

void Entities_ProcessPlayerInput(Player *player, float dt) {

    bool isMoving = false; 

    // ----------------------------------------------------------------
    // 0. VERIFICAÇÃO DE MORTE
    // ----------------------------------------------------------------
    if (player->health <= 0) {
        if (player->canJump) {
            if (player->state != PlayerDead) {
                player->position.y += 15.0f; 
                player->state = PlayerDead;
            }
            player->speed = 0; 
        } 
        else {
            player->state = PlayerJump; // Morte aérea: Transiciona para queda/knockback
        }
        return; // Bloqueia todo o resto se estiver morto
    }

    // ----------------------------------------------------------------
    // 1. INPUT DE ATAQUE (Prioridade Máxima)
    // ----------------------------------------------------------------
    // IsKeyPressed para não resetar a animação todo frame
    // 🚨 CORREÇÃO: Bloqueia ataque se player estiver invulnerável
    if (IsKeyPressed(KEY_Z) && !player->isatk && player->invulnerabilityTimer <= 0) {
        player->hasHit = false;
        player->isatk = true;
        player->state = PlayerAtk;
        
        // Reset manual da animação
        player->anim[PlayerAtk].indiceFrameX = 0;
        player->anim[PlayerAtk].indiceFrameY = 0;
        player->anim[PlayerAtk].temporizador = player->anim[PlayerAtk].tempoPorFrame;
        player->anim[PlayerAtk].final = false;

        PlaySound(player->soundPlayer.Atk);
    }

    // Se o jogador estiver no meio de um ataque, IGNORA movimentação (Prioridade)
    if (player->isatk) {
        player->state = PlayerAtk;
        if (player->anim[PlayerAtk].final) {
            player->isatk = false;
            player->state = PlayerIdle;
        }
        return; // Retorna cedo (ignora movimento/pulo/estado)
    }

    // ----------------------------------------------------------------
    // 2. INPUT DE MOVIMENTO (Atualiza isMoving)
    // ----------------------------------------------------------------
    
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
    // Bloqueia pulo durante invulnerabilidade/knockback
    if (IsKeyPressed(KEY_SPACE) && player->canJump && player->invulnerabilityTimer <= 0) {
        player->speed = -PLAYER_JUMP_SPD;
        player->canJump = false;
        player->state = PlayerJump;
        PlaySound(player->soundPlayer.Jump);
    }

    // ----------------------------------------------------------------
    // 4. DEFINIÇÃO DE ESTADO (Hierarquia de Prioridade)
    // ----------------------------------------------------------------
    
    // 1. PRIORIDADE: INVULNERABILIDADE/KNOCKBACK
    if (player->invulnerabilityTimer > 0) {
        if (!player->canJump) { // Se o player está no ar (knockback vertical)
            player->state = PlayerJump; 
        }
        else if (isMoving) {
        player->state = PlayerRun;
        }  
        else {
            player->state = PlayerIdle; // Se já aterrissou, mas ainda invulnerável
        }
    }
    // 2. PRIORIDADE: ESTADO AÉREO NORMAL (Jump/Fall)
    else if (!player->canJump) {
        player->state = PlayerJump;
    } 
    // 3. PRIORIDADE: MOVIMENTO HORIZONTAL
    else if (isMoving) {
        player->state = PlayerRun;
    } 
    // 4. ÚLTIMA PRIORIDADE: PARADO
    else {
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