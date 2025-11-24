#include "scene_manager.h"
#include "../data/entity_types.h"
#include "../data/map_data.h"
#include "../systems/systems.h"
#include "../entities/entities.h"
#include "../config/config.h"

static Player player;
static EnvItem envItems[] = {
    {{ 0, 0, 2280, 800 }, 0, DARKBLUE }, // "background"
    {{ 0, 400, 1280, 500 }, 1, DARKGREEN }, // chao 1
    {{ 1280, 450, 600, 400 }, 1, DARKGREEN }, // chao 2
    {{ 1880, 400, 400, 500 }, 1, DARKGREEN }, // chao 3
    {{ 250, 350, 100, 10 }, 1, DARKGREEN }, // retangulo alto 1
    {{ 650, 350, 100, 10 }, 1, DARKGREEN } // retangulo alto 2
};
static Camera2D camera;

void Gameplay_Init(void) {
    player.position = (Vector2){ 200, 400 };
    player.speed = 0;
    player.canJump = false;
    
    camera.target = player.position;
    camera.offset = (Vector2){ LARGURA_TELA/2.0f, ALTURA_TELA/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
}

EstadoJogo Gameplay_Update(void) {
    float dt = GetFrameTime();

    if (IsKeyPressed(KEY_ESCAPE)) return TELA_MENU;

    // 1. Entidade processa Input
    Entities_ProcessPlayerInput(&player, dt);
    
    // 2. Sistema de Física resolve colisões e movimento
    Physics_UpdatePlayer(&player, envItems, 5, dt);
    
    // 3. Atualiza Câmera
    int envItemsLength = sizeof(envItems)/sizeof(envItems[0]);
    Render_UpdateCamera(&camera, &player, envItems, envItemsLength, LARGURA_TELA, ALTURA_TELA);

    return TELA_GAMEPLAY;
}

void Gameplay_Draw(void) {
    ClearBackground(LIGHTGRAY);
    
    BeginMode2D(camera);
        Render_Map(envItems, 5);
        Render_Player(&player);
    EndMode2D();
    
    DrawText("Controles: Setas + Espaco | ESC para fechar", 20, 20, 20, BLACK);
}