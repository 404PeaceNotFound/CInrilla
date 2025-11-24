#include "scene_manager.h"
#include "../data/entity_types.h"
#include "../data/map_data.h"
#include "../systems/systems.h"
#include "../entities/entities.h"
#include "../config/config.h"
#include "../systems/enemy_system.h"

// player 
static Player player;
static EnvItem envItems[] = {
    {{ 0, 0, 1000, 400 }, 0, LIGHTGRAY },
    {{ 0, 400, 1000, 200 }, 1, GRAY },
    {{ 300, 200, 400, 10 }, 1, GRAY },
    {{ 250, 300, 100, 10 }, 1, GRAY },
    {{ 650, 300, 100, 10 }, 1, GRAY }
};
static Camera2D camera;

// inimigos

#define MAX_ENEMIES 10 
static Enemy enemies[MAX_ENEMIES];
static int enemyCount =0;
//colição inimigos x player
void CheckPlayerEnemyCollision(Player *p, Enemy *e) {
    if (!e->active) return;

    // Hitbox do Player (ajustada para ser menor que o sprite)
    Rectangle playerRect = { p->position.x - 20, p->position.y - 40, 40, 40 }; 
    // Hitbox do Inimigo
    Rectangle enemyRect = { e->position.x, e->position.y - e->height, e->width, e->height };

    if (CheckCollisionRecs(playerRect, enemyRect)) {
        if (p->position.x < e->position.x){
            p->position.x -=30;
        }else{
            p->position.x +=30;
        }
        p->speed -20;
    }
}
// init 
void Gameplay_Init(void) {
    Render_LoadAssets(); // chamar a função que geral as texturas
    //player
    player.position = (Vector2){ 600, 300 };
    player.speed = 0;
    player.canJump = false;
    // camera 
    camera.target = player.position;
    camera.offset = (Vector2){ LARGURA_TELA/2.0f, ALTURA_TELA/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.5f;

    //geração de inimigos

    enemyCount = 0;
    //javali
    enemies[enemyCount] = Enemy_Create((Vector2){500, 400}, 200, 500, 60); // inimigos no chão y=400
    Render_ConfigEnemy(&enemies[enemyCount], ENEMY_TYPE_BOAR);
    enemyCount++;
    //javali 2
    enemies[enemyCount] = Enemy_Create((Vector2){800, 400}, 700, 900, 60);
    Render_ConfigEnemy(&enemies[enemyCount], ENEMY_TYPE_BOAR);
    enemyCount++;
    //abelha
    float plataformay = envItems[2].rect.y;
    enemies[enemyCount]= Enemy_Create((Vector2){450, plataformay}, 300, 650, 60); // inimigos na plataforma 
    Render_ConfigEnemy(&enemies[enemyCount], ENEMY_TYPE_SMALL_BEE);
    enemyCount++;
}
// update 
EstadoJogo Gameplay_Update(void) {
    float dt = GetFrameTime();

    if (IsKeyPressed(KEY_ESCAPE)) return TELA_MENU;

    // 1. Entidade processa Input
    Entities_ProcessPlayerInput(&player, dt);
    
    // 2. Sistema de Física resolve colisões e movimento
    Physics_UpdatePlayer(&player, envItems, 5, dt);
    
    // 3. Atualiza Câmera
    Render_UpdateCamera(&camera, &player, LARGURA_TELA, ALTURA_TELA);

    //  4. Geração de inimigos
    for(int i =0; i < enemyCount; i++){
        //aplicação da gravidade 
        Physics_UpdateEnemy(&enemies[i], envItems, 5, dt);
        UpdateEnemy(&enemies[i], dt);
        CheckPlayerEnemyCollision(&player, &enemies[i]);
    }

    return TELA_GAMEPLAY;
}
//draw 
void Gameplay_Draw(void) {
    ClearBackground(LIGHTGRAY);
    
    BeginMode2D(camera);
        Render_Map(envItems, 5);
        Render_Player(&player);
        //render inimigos
        for(int i =0; i < enemyCount; i++){
            Render_Enemy(&enemies[i]);
        }
    EndMode2D();
    
    DrawText("Controles: Setas + Espaco | ESC para voltar", 20, 20, 20, BLACK);
}