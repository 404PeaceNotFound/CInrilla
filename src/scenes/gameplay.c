#include "scene_manager.h"
#include "../data/entity_types.h"
#include "../data/map_data.h"
#include "../systems/systems.h"
#include "../entities/entities.h"
#include "../config/config.h"

static Player player;
static EnvItem envItems[] = {
    {{ 0, 0, 1000, 400 }, 0, LIGHTGRAY },
    {{ 0, 400, 1000, 200 }, 1, GRAY },
    {{ 300, 200, 400, 10 }, 1, GRAY },
    {{ 250, 300, 100, 10 }, 1, GRAY },
    {{ 650, 300, 100, 10 }, 1, GRAY }
};
static Camera2D camera;

void Gameplay_Init(void) {
    player.position = (Vector2){ 600, 300 };
    player.speed = 0;
    player.canJump = false;
    player.isatk = false;

    initPlayer(&player);
    camera.target = player.position;
    camera.offset = (Vector2){LARGURA_TELA/2.0f, ALTURA_TELA/2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.5f;
}

EstadoJogo Gameplay_Update(void) { //Aqui rola a zuera de atualizar com o tempo
    float dt = GetFrameTime();

    if (IsKeyPressed(KEY_ESCAPE)) return TELA_MENU;

    // 1. Entidade processa Input
    Entities_ProcessPlayerInput(&player, dt);
    
    // 2. Sistema de Física resolve colisões e movimento
    Physics_UpdatePlayer(&player, envItems, 5, dt);
    
    // 3. Atualiza Câmera
    Render_UpdateCamera(&camera, &player, LARGURA_TELA, ALTURA_TELA);

    // 3. Atualiza Sprite
    Render_UpdateAnim(&player.anim[player.state], dt);

    return TELA_GAMEPLAY;
}

void Gameplay_Draw(void) { //Aqui rola a zuera do desenho 
    ClearBackground(LIGHTGRAY);
    
    BeginMode2D(camera);
        Render_Map(envItems, 5);
        Render_Player(&player);
    EndMode2D();
    
    DrawText("Controles: Setas + Espaco | ESC para voltar", 20, 20, 20, BLACK);
}