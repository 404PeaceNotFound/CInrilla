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
static int enemyCount= 0;

// init 
void Gameplay_Init(void) {
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

    enemies[enemyCount++] = Enemy_Create((Vector2){500, 400}, 400, 700, 60); // inimigos no chão 

    float plataformay = envItems[2].rect.y;
    enemies[enemyCount++]= Enemy_Create((Vector2){450, plataformay}, 300, 650, 80); // inimigos na plataforma 

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
        UpdateEnemy(&enemies[i], dt);
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
            //DrawEnemy(&enemies[i]);
            DrawRectangle(enemies[i].position.x, enemies[i].position.y, 40, 40, RED); // teste para ver se os inimigos estão sendogerados
            DrawCircle(enemies[i].position.x, enemies[i].position.y, 5, GREEN);
        }
    EndMode2D();
    
    DrawText("Controles: Setas + Espaco | ESC para voltar", 20, 20, 20, BLACK);
}