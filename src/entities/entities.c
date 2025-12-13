    #include "entities.h"
    #include "../systems/systems.h"
    #include "../data/entity_types.h"
    #include <raylib.h>

    // Input do Player
    void Entities_ProcessPlayerInput(Player *player, float dt) {

        // ----------------------------------------------------------------
        // 0. VERIFICAÇÃO DE MORTE
        // ----------------------------------------------------------------
        if (player->health <= 0) {
            if (player->canJump) {
                if (player->state != PlayerDead) { 
                    player->state = PlayerDead;
                }
                player->speed = 0; 
            } 
            else {
                player->state = PlayerJump; // Morte aérea: Transiciona para queda/knockback
            }
            return; // Bloqueia todo o resto se estiver morto
        }   
        
        // 1. Atualiza Timer de Invencibilidade
        if (player->hurtTimer > 0) {
            player->hurtTimer -= dt;
        }

        // Reset estado para Idle se não estiver fazendo nada
        if (!player->isatk && player->canJump) {
            player->state = PlayerIdle;
        }

        // --- CORREÇÃO AQUI ---
        // Só permitimos processar movimento lateral se NÃO estiver atacando (!player->isatk)
        // Se quiser permitir movimento no ar enquanto ataca, adicione "|| !player->canJump"
        if (!player->isatk) { 
            
            // Movimento Lateral
            if (IsKeyDown(KEY_RIGHT)) {
                player->position.x += 300.0f * dt; 
                player->PlayerDirection = 1;
                if (player->canJump) player->state = PlayerRun;
            }
            if (IsKeyDown(KEY_LEFT)) {
                player->position.x -= 300.0f * dt;
                player->PlayerDirection = -1;
                if (player->canJump) player->state = PlayerRun;
            }
        }
        // ---------------------

        // Pulo (Também bloqueamos o pulo se estiver atacando no chão, para evitar cancelamento de animação)
        if (IsKeyPressed(KEY_SPACE) && player->canJump && !player->isatk) {
            player->speed = -500.0f; 
            player->canJump = false;
            
            if(IsSoundValid(player->soundPlayer.Jump)) PlaySound(player->soundPlayer.Jump);
        }

        // Estado de Pulo
        if (!player->canJump && !player->isatk) {
            player->state = PlayerJump;
        }

        // Ataque
        if (IsKeyPressed(KEY_Z) && !player->isatk) {
            player->isatk = true;
            player->hasHit = false;
            player->state = PlayerAtk;
            
            // Garante que o índice comece do zero
            player->anim[PlayerAtk].indiceFrameX = 0; 
            player->anim[PlayerAtk].temporizador = 0; 
            player->anim[PlayerAtk].final = false;    
            
            if(IsSoundValid(player->soundPlayer.Atk)) PlaySound(player->soundPlayer.Atk);
        }

        // Controle de fim de ataque
        if (player->isatk) {
            if (player->anim[PlayerAtk].final) {
                player->isatk = false;
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