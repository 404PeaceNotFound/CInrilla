#include "scene_manager.h"
#include "../data/entity_types.h"
#include "../data/map_data.h"
#include "../systems/systems.h"
#include "../entities/entities.h"
#include "../config/config.h"
#include "../ui/ui.h"

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
    player.health = 100;
    camera.target = player.position;
    camera.offset = (Vector2){ LARGURA_TELA/2.0f, ALTURA_TELA/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.5f;
}

EstadoJogo Gameplay_Update(void) {
    float dt = GetFrameTime();

    if (IsKeyPressed(KEY_BACKSPACE)) return TELA_PAUSA;

    // 1. Entidade processa Input
    Entities_ProcessPlayerInput(&player, dt);
    
    // 2. Sistema de Física resolve colisões e movimento
    Physics_UpdatePlayer(&player, envItems, 5, dt);
    
    // 3. Atualiza Câmera
    Render_UpdateCamera(&camera, &player, LARGURA_TELA, ALTURA_TELA);

    return TELA_GAMEPLAY;
}

void Gameplay_Draw(void) {
    ClearBackground(LIGHTGRAY);

    BeginMode2D(camera);
        Render_Map(envItems, 5);
        Render_Player(&player);
    EndMode2D();
    
    UI_DesenharHealthBar(player.health,100,LARGURA_TELA);

    DrawText("Controles: Setas + Espaco | BACKSPACE para pausa", 20, 20, 20, BLACK);
}