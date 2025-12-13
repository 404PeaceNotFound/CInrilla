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
            if (p->position.x < e->position.x) e->position.x += 30;
            else e->position.x -= 30;
            
            e->active = 0; 
            p->hasHit = true; 
            return; 
        }
    }

    // 2. DANO NO PLAYER (Inimigo toca no player)
    if (p->hurtTimer <= 0 && CheckCollisionRecs(playerBody, enemyRect)) {
        
        p->health -= 1; 

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
    // --- CORREÇÃO FUNDAMENTAL ---
    // Atualiza a variável global para saber em qual nível estamos de verdade
    nivelAtual = nivel; 
    // ----------------------------

    if (gameMap.loaded) MapLoader_Unload(&gameMap);
    
    enemyCount = 0; 

    // Verificação de segurança
    if (nivel > MAX_NIVEIS) {
        nivelAtual = 1;
        nivel = 1;
    }

    // Carregar Mapa
    gameMap = MapLoader_Load(caminhosMapas[nivel - 1]);

    // Resetar Player
// 1. Resetar Vida (Crucial!)
    player.health = 10;
    
    // 2. Resetar Física e Estado
    player.speed = 0;
    player.canJump = false;
    player.isatk = false;
    player.state = PlayerIdle;
    player.hurtTimer = 0; 
    player.PlayerDirection = 1;
    
    // 3. Resetar Animação de Morte
    // Isso impede que o jogo ache que a animação já acabou
    player.anim[PlayerDead].final = false;     
    player.anim[PlayerDead].indiceFrameX = 0;
    player.anim[PlayerDead].temporizador = 0;
    
    // =========================================================================
    // CONFIGURAÇÃO DE SPAWN (PLAYER E INIMIGOS)
    // =========================================================================
    
    if (nivel == 1) {
        // --- FASE 1 (120x20 tiles) ---
        // Player: X=2, Y=7
        player.renderoffsetY = 32.0f;
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
        player.renderoffsetY = 20.0f;
        player.position = (Vector2){ 
            9 * 16,   //X
            40 * 16    
        };

        // Inimigos Fase 2 (Ajustados para o mapa de 90 tiles largura)
        
        // Abelha alta no início (X=20, Y=5)
        enemies[enemyCount] = Enemy_Create((Vector2){20 * 16, 40 * 16}, 15 * 16, 25 * 16, 50);
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
}

void Gameplay_Reiniciar(void) {
    // Se quiser reiniciar do Nível 1:
    CarregarNivel(1); 
    
    // OU se quiser reiniciar do nível que morreu (Checkpoint):
    //CarregarNivel(nivelAtual); 
}

void Gameplay_ProximoNivel(void) {
    nivelAtual++;
    
    // Segurança: se passar do máximo, volta pro 1 (ou trata como quiser)
    if (nivelAtual > MAX_NIVEIS) {
        nivelAtual = 1;
    }
    
    CarregarNivel(nivelAtual);
}