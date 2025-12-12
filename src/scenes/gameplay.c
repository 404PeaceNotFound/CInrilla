#include "scene_manager.h"
#include "../config/config.h"
#include "../data/map_data.h"
#include "../data/map_loader.h"
#include "../data/entity_types.h"
#include "../entities/entities.h"
#include "../systems/systems.h"
#include "../ui/ui.h"

// Variáveis Estáticas
static Player player;
static GameMap gameMap;
static Camera2D camera;

// Controle de Níveis
static int nivelAtual = 1;
#define MAX_NIVEIS 2
const char* caminhosMapas[MAX_NIVEIS] = {
    "assets/maps/maptest.json",   // Nível 1
    "assets/maps/fase2.json"      // Nível 2
};

// Inimigos
#define MAX_ENEMIES 20 
static Enemy enemies[MAX_ENEMIES];
static int enemyCount = 0;

// -----------------------------------------------------------------------------
// Lógica de Colisão Player vs Inimigo
bool CheckPlayerEnemyCollision(Player *p, Enemy *e) {
    if (!e->active) return false;

    // Hitboxes
    Rectangle playerBody = { p->position.x - 20, p->position.y - 40, 40, 40 }; 
    Rectangle enemyRect = { e->position.x, e->position.y - e->height, e->width, e->height };

    // 1. ATAQUE DO PLAYER (Causa dano no inimigo)
    if (p->isatk && !p->hasHit) {
        float atkRange = 50.0f;
        float atkHeight = 40.0f;
        float attackX = (p->PlayerDirection == 1) ? p->position.x + 20 : p->position.x - 20 - atkRange;

        Rectangle atkRect = { attackX, p->position.y - 40, atkRange, atkHeight };

        if (CheckCollisionRecs(atkRect, enemyRect)) {
            // Knockback no Inimigo
            if (p->position.x < e->position.x) e->position.x += 30;
            else e->position.x -= 30;
            
            // Mata inimigo
            e->active = 0; 
            // e->verticalSpeed = -100.0f; (Desnecessário se active = 0)
            
            p->hasHit = true; 
            return false; 
        }
    }

    // 2. DANO NO PLAYER (Inimigo toca no player)
    // Só acontece se o player NÃO estiver invencível (hurtTimer <= 0)
    if (p->hurtTimer <= 0 && CheckCollisionRecs(playerBody, enemyRect)) {
        
        p->health -= 1;  // Dano
        p->speed = -300; // Pulo de dano (Aumentei um pouco para garantir que saia do chão)
        p->canJump = false; // Garante que ele entre no estado de Pulo
        
        // Ativa Invencibilidade (1 segundo)
        p->hurtTimer = 1.0f;

        // Knockback no Player (Seguro)
        float knockbackDist = 30.0f; 
        float dir = (p->position.x < e->position.x) ? -1.0f : 1.0f; 
        p->position.x += (knockbackDist * dir);
        
        if (p->health <= 0) return true; // Morreu -> GameOver
    }
    
    return false;
}

// -----------------------------------------------------------------------------
// Lógica Fase
void CarregarNivel(int nivel) {
    if (gameMap.loaded) MapLoader_Unload(&gameMap);
    
    enemyCount = 0; 

    if (nivel > MAX_NIVEIS) {
        nivelAtual = 1;
        nivel = 1;
    }

    // Carregar Mapa
    gameMap = MapLoader_Load(caminhosMapas[nivel - 1]);

    // Resetar Player
    player.speed = 0;
    player.canJump = false;
    player.isatk = false;
    player.state = PlayerIdle;
    player.hurtTimer = 0; // Reseta invencibilidade
    
    // =========================================================================
    // CONFIGURAÇÃO DE SPAWN (PLAYER E INIMIGOS)
    // =========================================================================
    
    if (nivel == 1) {
        // --- FASE 1 (120x20 tiles) ---
        // Player: X=2, Y=7
        player.position = (Vector2){ 2 * 16, 7 * 16 };

        // Inimigos Fase 1
        enemies[enemyCount] = Enemy_Create((Vector2){40 * 16, 12 * 16}, 35 * 16, 45 * 16, 60); 
        Render_ConfigEnemy(&enemies[enemyCount], ENEMY_TYPE_BOAR); 
        enemyCount++;

        enemies[enemyCount] = Enemy_Create((Vector2){90 * 16, 10 * 16}, 85 * 16, 95 * 16, 60); 
        Render_ConfigEnemy(&enemies[enemyCount], ENEMY_TYPE_SMALL_BEE);
        enemyCount++;
        
        // Inimigo Final Fase 1
        enemies[enemyCount] = Enemy_Create((Vector2){110 * 16, 12 * 16}, 105 * 16, 115 * 16, 80); 
        Render_ConfigEnemy(&enemies[enemyCount], ENEMY_TYPE_BOAR); 
        enemyCount++;
    } 
    else if (nivel == 2) {
        // --- FASE 2 (90x50 tiles) ---
        // Player: X=9, Y=7 (Conforme solicitado)
        // Y = 7 * 16 = 112 pixels.
        player.position = (Vector2){ 
            9 * 16,   
            7 * 16    
        };

        // Inimigos Fase 2 (Ajustados para o mapa de 90 tiles largura)
        
        // Abelha alta no início (X=20, Y=5)
        enemies[enemyCount] = Enemy_Create((Vector2){20 * 16, 5 * 16}, 15 * 16, 25 * 16, 50);
        Render_ConfigEnemy(&enemies[enemyCount], ENEMY_TYPE_SMALL_BEE);
        enemyCount++;

        // Javali no meio do mapa (X=45, Y=20 - descendo)
        enemies[enemyCount] = Enemy_Create((Vector2){45 * 16, 20 * 16}, 40 * 16, 50 * 16, 70);
        Render_ConfigEnemy(&enemies[enemyCount], ENEMY_TYPE_BOAR);
        enemyCount++;
        
        // Javali no final (X=80, Y=40 - bem baixo)
        enemies[enemyCount] = Enemy_Create((Vector2){80 * 16, 40 * 16}, 75 * 16, 85 * 16, 90);
        Render_ConfigEnemy(&enemies[enemyCount], ENEMY_TYPE_BOAR);
        enemyCount++;
    }
}

// -----------------------------------------------------------------------------
// Inicialização
void Gameplay_Init(void) {
    Render_LoadAssets();
    initPlayer(&player); 
    
    nivelAtual = 1;
    CarregarNivel(nivelAtual);

    camera.target = player.position;
    camera.offset = (Vector2){LARGURA_TELA/2.0f, ALTURA_TELA/2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 2.2f;
}

// -----------------------------------------------------------------------------
// Update
EstadoJogo Gameplay_Update(void) {
    float dt = GetFrameTime();

    if (IsKeyPressed(KEY_ESCAPE)) return TELA_MENU;

    // 1. Input e Animação do Player
    Entities_ProcessPlayerInput(&player, dt);
    Physics_UpdatePlayer(&player, &gameMap, dt);
    Render_UpdatePlayerAnim(&player, dt); // IMPORTANTÍSSIMO PARA O ATAQUE FUNCIONAR

    // 2. Inimigos
    int inimigosVivos = 0; 
    for(int i = 0; i < enemyCount; i++) {
        if (enemies[i].active) {
            inimigosVivos++;
            Physics_UpdateEnemy(&enemies[i], &gameMap, dt);
            
            // Retorna Gameover se morrer
            if (CheckPlayerEnemyCollision(&player, &enemies[i])) {
                return TELA_GAMEOVER;
            }
        }
    }

    // 3. Transição de Fase
    if (inimigosVivos == 0) {
        nivelAtual++;
        if (nivelAtual > MAX_NIVEIS) {
            return TELA_WINNER; 
        } else {
            CarregarNivel(nivelAtual);
        }
    }

    // 4. Câmera
    Render_UpdateCamera(&camera, &player, &gameMap, LARGURA_TELA, ALTURA_TELA);

    return TELA_GAMEPLAY;
}

// -----------------------------------------------------------------------------
// Draw
void Gameplay_Draw(void) {
    ClearBackground(SKYBLUE); 

    BeginMode2D(camera);
        Render_Map(&gameMap);    
        
        for(int i = 0; i < enemyCount; i++){
            Render_Enemy(&enemies[i]);
        }
        
        Render_Player(&player);  
    EndMode2D();

    UI_DesenharHealthBar(player.health, 15, LARGURA_TELA); 
    DrawText(TextFormat("Nivel: %d | Inimigos: %d", nivelAtual, enemyCount), 20, 80, 20, BLACK);
    
    // Debug Posição
    // DrawText(TextFormat("X: %.0f Y: %.0f", player.position.x, player.position.y), 20, 110, 20, RED);
}

void Gameplay_Deinit(void) {
    MapLoader_Unload(&gameMap);
    Render_UnloadAssets();
}