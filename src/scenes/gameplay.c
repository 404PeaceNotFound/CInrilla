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

// Inimigos
#define MAX_ENEMIES 10 
static Enemy enemies[MAX_ENEMIES];
static int enemyCount = 0;

// -----------------------------------------------------------------------------
// Lógica de Colisão Player vs Inimigo
// MUDANÇA: Retorna bool (true se morreu/gameover) em vez de void
// -----------------------------------------------------------------------------
void CheckPlayerEnemyCollision(Player *p, Enemy *e) {
    if (!e->active) return;

    // 1. Áreas de Colisão
    Rectangle playerBody = { p->position.x - 20, p->position.y - 40, 40, 40 }; 
    Rectangle enemyRect = { e->position.x, e->position.y - e->height, e->width, e->height };

    // ---------------------------------------------------------
    // ETAPA 1: VERIFICA O ATAQUE (Hitbox da Arma)
    // ---------------------------------------------------------
    // ATENÇÃO: Isso agora fica FORA do CheckCollisionRecs(playerBody...)
    if(p->isatk && !p->hasHit){
        float atkRange = 50.0f;
        float atkHeight = 40.0f;
        float attackX;

        // Define onde a espada "nasce"
        if(p->PlayerDirection == 1){
            attackX = p->position.x + 20; // Na frente (Direita)
        }
        else {
            attackX = p->position.x - 20 - atkRange; // Na frente (Esquerda)
        }

        Rectangle atkRect = {
            attackX,
            p->position.y - 40,
            atkRange,
            atkHeight,
        };

    if (CheckCollisionRecs(atkRect, enemyRect)) {
            
            e->health -= p->damage; // Aplica o Dano real
            
            // Garante que o dano só seja aplicado uma vez por ataque
            p->hasHit = true; 
            
            // Aplica o Knockback Horizontal (Empurrão)
            if (e->health > 0) {
                // Determine a direção do empurrão
                float knockbackAmount = 40.0f; 
                e->position.x += (p->position.x < e->position.x) ? knockbackAmount : -knockbackAmount; 
                
                // Aplica Knockback Vertical (Pulo) se necessário
                e->verticalSpeed = -100.0f; // Exemplo de pulo de dano
            } else {
                 e->active = 0; // Temporário: Garante que o inimigo suma após a morte
            }
            
            return; // O inimigo foi atingido, não verifica mais colisões neste frame.
        }
    }

    // ---------------------------------------------------------
    // ETAPA 2: VERIFICA COLISÃO DE CORPO (Dano no Player)
    // ---------------------------------------------------------
    if (CheckCollisionRecs(playerBody, enemyRect)) {
            if (p->invulnerabilityTimer <= 0) 
            { 
        // PLAYER TOMOU DANO
                p->health -= e->damage; // Tira vida
                p->invulnerabilityTimer = 2.0f; // Tempo de invulnerabilidade
                p->canJump = false; // Força o estado aéreo para knockback

                // Empurrão no Player (Knockback)
                if (p->position.x < e->position.x) {
                    p->position.x -= 50; // Player vai para Esquerda
                } else {
                    p->position.x += 50; // Player vai para Direita
                }
                
                p->speed = -200; // Pulo pequeno de dano (Knockback vertical)
            }
        }   
    }

// -----------------------------------------------------------------------------
// Inicialização
// -----------------------------------------------------------------------------
void Gameplay_Init(void) {
    Render_LoadAssets();

    // 1. Mapa
    gameMap = MapLoader_Load("assets/maps/maptest.json");

    // 2. Player
    // CORREÇÃO: Converter tile (7,12) para pixels ou usar valor fixo em pixels para evitar bugar na parede
    // Se tileWidth for 16: 7*16 = 112, 12*16 = 192.
    player.position = (Vector2){ 
    7 * gameMap.tileWidth,  // 7 * 16 = 112
    12 * gameMap.tileHeight // 12 * 16 = 192
    };
    player.speed = 0;
    player.canJump = false;
    player.isatk = false;

    initPlayer(&player);

    // 3. Câmera
    camera.target = player.position;
    camera.offset = (Vector2){LARGURA_TELA/2.0f, ALTURA_TELA/2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 2.2f;

    // 4. Inimigos
    enemyCount = 0;

    // Inimigo 1: Javali
    enemies[enemyCount] = Enemy_Create((Vector2){600, 400}, 200, 500, 60); 
    Render_ConfigEnemy(&enemies[enemyCount], ENEMY_TYPE_BOAR); 
    enemyCount++;

    // Inimigo 2: Javali
    enemies[enemyCount] = Enemy_Create((Vector2){800, 400}, 700, 900, 60);
    Render_ConfigEnemy(&enemies[enemyCount], ENEMY_TYPE_BOAR);
    enemyCount++;

    // Inimigo 3: Abelha
    enemies[enemyCount] = Enemy_Create((Vector2){800, 200}, 200, 500, 60); 
    Render_ConfigEnemy(&enemies[enemyCount], ENEMY_TYPE_SMALL_BEE);
    enemyCount++;
}

// -----------------------------------------------------------------------------
// Update
// -----------------------------------------------------------------------------
EstadoJogo Gameplay_Update(void) {
    float dt = GetFrameTime();
    static bool resetGame = true;

    if (resetGame) {
        // Reset Completo do Player 
        player.health = 5;
        player.state = PlayerIdle;
        player.isatk = false;
        player.speed = 0.0f;
        player.canJump = true; // Importante para não começar caindo sem controle
        player.anim[PlayerDead].final = false;    
        
        // CORREÇÃO: Usar a mesma posição lógica do Init
        // Se o seu mapa tem tiles de 16x16 ou 32x32, calcule de acordo.
        // Vou usar a mesma lógica que estava no seu Init:
        if (gameMap.loaded) {
            player.position = (Vector2){ 7 * gameMap.tileWidth, 12 * gameMap.tileHeight };
        } else {
            // Fallback caso o mapa não tenha carregado ainda (improvável, mas seguro)
            player.position = (Vector2){ 100, 100 }; 
        }

        resetGame = false;
    }
    // 1. Input Player
    Entities_ProcessPlayerInput(&player, dt);
    
    // 2. Física Player
    Physics_UpdatePlayer(&player, &gameMap, dt);

    Render_UpdateAnim(&player.anim[player.state], dt);

    //
                if (IsKeyPressed(KEY_BACKSPACE)) return TELA_PAUSA;
            
            if (player.state == PlayerDead) {
                if (player.anim[player.state].final) {
                    resetGame = true;   
                    return TELA_GAMEOVER;
                    }

                    return TELA_GAMEPLAY;
                }
            
            if (IsKeyPressed(KEY_ESCAPE)){
                resetGame = true;
                return TELA_MENU;
            }

            if(player.anim[player.state].final){
                player.state = PlayerIdle;              
                }

            if (player.invulnerabilityTimer > 0) {
                player.invulnerabilityTimer -= dt;
                }

    // 3. Física Inimigos e Colisões
    for(int i = 0; i < enemyCount; i++) {
        // Atualiza Movimento do Inimigo
        Physics_UpdateEnemy(&enemies[i], &gameMap, dt);
        Enemy_UpdateAnim(&enemies[i], dt);
        // Verifica Colisão com Player (Dano/Ataque)
        CheckPlayerEnemyCollision(&player, &enemies[i]);

        
    }

    // 4. Câmera
    Render_UpdateCamera(&camera, &player, &gameMap, LARGURA_TELA, ALTURA_TELA);

    return TELA_GAMEPLAY;
}

// -----------------------------------------------------------------------------
// Draw
// -----------------------------------------------------------------------------
void Gameplay_Draw(void) {
    // CORREÇÃO: Apenas um ClearBackground
    ClearBackground(SKYBLUE); 

    BeginMode2D(camera);
        Render_Map(&gameMap);    
        
        // Desenha inimigos
        for(int i = 0; i < enemyCount; i++){
            Render_Enemy(&enemies[i]);
        }
        
        Render_Player(&player);  
    EndMode2D();

    // UI
    UI_DesenharHealthBar(player.health, 15, LARGURA_TELA); // Assumindo MaxHealth = 15 conforme initPlayer
    DrawText("Controles: Setas + Espaco | Z para Atacar", 20, 100, 20, BLACK);
    DrawText(TextFormat("Pos: %.0f, %.0f", player.position.x, player.position.y), 10, 150, 20, RED);
}

void Gameplay_Deinit(void) {
    MapLoader_Unload(&gameMap);
    Render_UnloadAssets();
}