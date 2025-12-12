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
bool CheckPlayerEnemyCollision(Player *p, Enemy *e) {
    if (!e->active) return false;

    // Hitboxes
    Rectangle playerBody = { p->position.x - 20, p->position.y - 40, 40, 40 }; 
    Rectangle enemyRect = { e->position.x, e->position.y - e->height, e->width, e->height };

    // 1. ATAQUE DO PLAYER
    if (p->isatk && !p->hasHit) {
        float atkRange = 50.0f;
        float atkHeight = 40.0f;
        float attackX = (p->PlayerDirection == 1) ? p->position.x + 20 : p->position.x - 20 - atkRange;

        Rectangle atkRect = { attackX, p->position.y - 40, atkRange, atkHeight };

        if (CheckCollisionRecs(atkRect, enemyRect)) {
            // Knockback no Inimigo
            if (p->position.x < e->position.x) e->position.x += 30;
            else e->position.x -= 30;
            
            // Mata inimigo (simples)
            e->active = 0; 
            e->verticalSpeed = -100.0f; 
            
            p->hasHit = true; 
            return false; 
        }
    }

    // 2. CORPO A CORPO (Dano no Player)
    if (CheckCollisionRecs(playerBody, enemyRect)) {
        p->health -= 1; // Dano
        p->speed = -200; // Pulo de dano
        

        // Knockback no Player
        //if (p->position.x < e->position.x) p->position.x -= 50;
        //else p->position.x += 50;
        float knockbackDist = 30.0f; // Reduzi de 50 para 30 para ser mais seguro
        float dir = (p->position.x < e->position.x) ? -1.0f : 1.0f; // -1 esq, 1 dir

        p->position.x += (knockbackDist * dir);
        
        
        
        if (p->health <= 0) return true; // Morreu -> GameOver
    }
    
    return false;
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

    if (IsKeyPressed(KEY_ESCAPE)) return TELA_MENU;

    // 1. Input Player
    Entities_ProcessPlayerInput(&player, dt);
    
    // 2. Física Player
    Physics_UpdatePlayer(&player, &gameMap, dt);

    // 3. Física Inimigos e Colisões
    for(int i = 0; i < enemyCount; i++) {
        // Atualiza Movimento do Inimigo
        Physics_UpdateEnemy(&enemies[i], &gameMap, dt);
        
        // Verifica Colisão com Player (Dano/Ataque)
        if (CheckPlayerEnemyCollision(&player, &enemies[i])) {
            return TELA_GAMEOVER; // Se retornar true, player morreu
        }
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