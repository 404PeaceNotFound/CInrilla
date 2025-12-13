#include "scene_manager.h"
#include "../config/config.h"
#include "../data/map_data.h"
#include "../data/map_loader.h"
#include "../data/entity_types.h"
#include "../entities/entities.h"
#include "../systems/systems.h"
#include "../ui/ui.h"
#include <stddef.h>

// Variáveis Estáticas
static Player player;
static GameMap gameMap;
static Camera2D camera;
static Music musicFase1;
static Music musicFase2;
static Music* musicaAtual = NULL;

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
void CheckPlayerEnemyCollision(Player *p, Enemy *e) {
    if (!e->active) return; // Void function não retorna true/false

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
            if (p->position.x < e->position.x) e->position.x += 20;
            else e->position.x -= 20;
            
            e->health -= p->damage; 
            if (e->health <= 0) {
                e->active = 0; 
            }
            p->hasHit = true; 
            return; 
        }
    }

    // 2. DANO NO PLAYER (Inimigo toca no player)
    if (p->hurtTimer <= 0 && CheckCollisionRecs(playerBody, enemyRect)) {
        
        p->health -= e->damage; 

        // --- VERIFICAÇÃO DE MORTE ---
        if (p->health <= 0) {
            p->health = 0;
            p->state = PlayerDead; // Define estado de morte
            
            // Reinicia a animação de morte para garantir que toque do início
            p->anim[PlayerDead].indiceFrameX = 0;
            p->anim[PlayerDead].temporizador = 0;
            p->anim[PlayerDead].final = false;
            
        } else {
            // Se ainda estiver vivo, aplica knockback e invencibilidade
            p->speed = -300; 
            p->canJump = false; 
            p->hurtTimer = 1.0f;

            float knockbackDist = 30.0f; 
            float dir = (p->position.x < e->position.x) ? -1.0f : 1.0f; 
            p->position.x += (knockbackDist * dir);
        }
    }
}

// -----------------------------------------------------------------------------
// Lógica Fase
void CarregarNivel(int nivel) {
    // 1. Atualiza nível atual
    nivelAtual = nivel; 

    // 2. Limpa mapa anterior
    if (gameMap.loaded) MapLoader_Unload(&gameMap);
    
    enemyCount = 0; 

    // Segurança
    if (nivel > MAX_NIVEIS) {
        nivelAtual = 1;
        nivel = 1;
    }

    // 3. SISTEMA DE MÚSICA (Faltava isso!)
    // =================================================================
    // A. Para a música anterior (se houver)
    if (musicaAtual != NULL && musicaAtual->ctxData != NULL) {
        StopMusicStream(*musicaAtual);
    }

    // B. Escolhe a nova música
    musicaAtual = NULL; // Reseta para evitar erros
    
    if (nivel == 1) {
        if (musicFase1.ctxData != NULL) musicaAtual = &musicFase1;
    } 
    else if (nivel == 2) {
        if (musicFase2.ctxData != NULL) musicaAtual = &musicFase2;
    }

    // C. Toca a nova música
    if (musicaAtual != NULL) {
        PlayMusicStream(*musicaAtual);
        SetMasterVolume(1.0f);
    }
    // =================================================================

    // 4. Carrega Mapa e Reseta Player
    gameMap = MapLoader_Load(caminhosMapas[nivel - 1]);

    player.health = 10;
    player.speed = 0;
    player.canJump = false;
    player.isatk = false;
    player.state = PlayerIdle;
    player.hurtTimer = 0; 
    player.PlayerDirection = 1;
    
    player.anim[PlayerDead].final = false;     
    player.anim[PlayerDead].indiceFrameX = 0;
    player.anim[PlayerDead].temporizador = 0;
    
    // 5. CONFIGURAÇÃO DE SPAWN
    if (nivel == 1) {
        // --- FASE 1 ---
        player.renderoffsetY = 32.0f;
        player.position = (Vector2){ 2 * 16, 7 * 16 };

        enemies[enemyCount] = Enemy_Create((Vector2){40 * 16, 12 * 16}, 35 * 16, 45 * 16, 60); 
        Render_ConfigEnemy(&enemies[enemyCount], ENEMY_TYPE_BOAR); 
        enemies[enemyCount].health = 4;
        enemies[enemyCount].damage = 1;
        enemyCount++;

        enemies[enemyCount] = Enemy_Create((Vector2){90 * 16, 10 * 16}, 85 * 16, 95 * 16, 60); 
        Render_ConfigEnemy(&enemies[enemyCount], ENEMY_TYPE_SMALL_BEE);
        enemies[enemyCount].health = 4;
        enemies[enemyCount].damage = 1;
        enemyCount++;
        
        enemies[enemyCount] = Enemy_Create((Vector2){110 * 16, 12 * 16}, 105 * 16, 115 * 16, 80); 
        Render_ConfigEnemy(&enemies[enemyCount], ENEMY_TYPE_BOAR); 
        enemies[enemyCount].health = 4;
        enemies[enemyCount].damage = 1;
        enemyCount++;
    } 
    else if (nivel == 2) {
            // --- FASE 2 (90x50 tiles) ---
            player.renderoffsetY = 20.0f;
            player.position = (Vector2){ 9 * 16, 40 * 16 };

            // Inimigo 1: Abelha (Início) - Mantém igual
            enemies[enemyCount] = Enemy_Create((Vector2){20 * 16, 40 * 16}, 15 * 16, 25 * 16, 50);
            Render_ConfigEnemy(&enemies[enemyCount], ENEMY_TYPE_SMALL_BEE);
            enemies[enemyCount].health = 4;
            enemies[enemyCount].damage = 1;
            enemies[enemyCount].renderoffsetY = -5.0f;
            enemyCount++;

            // ==========================================================
            // BOSS: O JAVALI GIGANTE (Agora no meio do mapa)
            // ==========================================================
            enemies[enemyCount] = Enemy_Create((Vector2){55 * 16, 20 * 16}, 45 * 16, 70 * 16, 100);
            Render_ConfigEnemy(&enemies[enemyCount], ENEMY_TYPE_BOAR);
            enemies[enemyCount].health = 8;
            enemies[enemyCount].damage = 2;

            // Aumenta o tamanho
            float escalaBoss = 2.5f;
            enemies[enemyCount].width *= escalaBoss;
            enemies[enemyCount].height *= escalaBoss;

            enemies[enemyCount].position.y -= 50.0f; 
            
            // Ajuste visual 
            enemies[enemyCount].renderoffsetY = 12.0f; 

            enemyCount++;
            // ==========================================================
        }
}

// -----------------------------------------------------------------------------
// Inicialização
void Gameplay_Init(void) {
    Render_LoadAssets();

    if (FileExists("assets/sounds/OST/04 - Silent Forest.ogg")) {
        musicFase1 = LoadMusicStream("assets/sounds/OST/04 - Silent Forest.ogg");
        musicFase1.looping = true;
    }
    
    if (FileExists("assets/sounds/OST/20 - The Journey.ogg")) {
        musicFase2 = LoadMusicStream("assets/sounds/OST/20 - The Journey.ogg");
        musicFase2.looping = true;
    }

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
    if (musicaAtual != NULL && musicaAtual->ctxData != NULL) {
            UpdateMusicStream(*musicaAtual);
        }
    if (IsKeyPressed(KEY_ESCAPE)) return TELA_MENU;

    // --- 1. LÓGICA DE MORTE ---
    // Se o player morreu, tocamos a animação e trocamos de tela
    if (player.state == PlayerDead) {
        Render_UpdatePlayerAnim(&player, dt); // Atualiza só a animação
        
        // Se a animação de morte acabou
        if (player.anim[PlayerDead].final) {
            return TELA_GAMEOVER; // Vai para a tela de Game Over
        }
        return TELA_GAMEPLAY; // Continua na gameplay até a animação acabar
    }

    // --- 2. GAMEPLAY NORMAL (Só executa se estiver vivo) ---

    // Input e Física
    Entities_ProcessPlayerInput(&player, dt);
    Physics_UpdatePlayer(&player, &gameMap, dt);
    Render_UpdatePlayerAnim(&player, dt);

    // Inimigos
    int inimigosVivos = 0; 
    for(int i = 0; i < enemyCount; i++) {
        if (enemies[i].active) {
            inimigosVivos++;
            Physics_UpdateEnemy(&enemies[i], &gameMap, dt);
            Render_UpdateEnemyAnim(&enemies[i], dt);
            
            // Checa colisão
            CheckPlayerEnemyCollision(&player, &enemies[i]);
        }
    }

    // Transição de Fase (Só se estiver vivo)
if (inimigosVivos == 0) {
        // Verifica se é a última fase
        if (nivelAtual >= MAX_NIVEIS) {
            return TELA_WINNER; 
        } 
        
        // AQUI ESTÁ A MUDANÇA:
        // Em vez de carregar direto, avisamos o SceneManager para mostrar a transição
        return TELA_TRANSICAO; 
    }

    // Câmera
    Render_UpdateCamera(&camera, &player, &gameMap, LARGURA_TELA, ALTURA_TELA);

    return TELA_GAMEPLAY;
}
// -----------------------------------------------------------------------------
// Draw
void Gameplay_Draw(void) {
    ClearBackground(SKYBLUE); 
    Render_DrawBackground(LARGURA_TELA, ALTURA_TELA);
    BeginMode2D(camera);
        Render_Map(&gameMap);    
        
        for(int i = 0; i < enemyCount; i++){
            Render_Enemy(&enemies[i]);
        }
        
        Render_Player(&player);  
    EndMode2D();

    UI_DesenharHealthBar(player.health, 10, LARGURA_TELA); 
    DrawText(TextFormat("Nivel: %d | Inimigos: %d", nivelAtual, enemyCount), 20, 80, 20, BLACK);
    
    // Debug Posição
    // DrawText(TextFormat("X: %.0f Y: %.0f", player.position.x, player.position.y), 20, 110, 20, RED);
}

void Gameplay_Deinit(void) {
    MapLoader_Unload(&gameMap);
    Render_UnloadAssets();
    if (musicFase1.ctxData != NULL) UnloadMusicStream(musicFase1);
    if (musicFase2.ctxData != NULL) UnloadMusicStream(musicFase2);
}

void Gameplay_Reiniciar(void) {
    CarregarNivel(1); 
    
}

void Gameplay_ProximoNivel(void) {
    nivelAtual++;
    
    // Segurança: se passar do máximo, volta pro 1 (ou trata como quiser)
    if (nivelAtual > MAX_NIVEIS) {
        nivelAtual = 1;
    }
    
    CarregarNivel(nivelAtual);
}