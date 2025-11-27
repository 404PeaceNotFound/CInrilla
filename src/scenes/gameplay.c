#include "scene_manager.h"
#include "../config/config.h"
#include "../data/map_data.h"
#include "../data/map_loader.h"
#include "../systems/systems.h"
#include "../entities/entities.h"

static Player player;
static GameMap gameMap; // Nova struct de mapa
static Camera2D camera;

void Gameplay_Init(void) {
    // 1. Carregar Mapa do JSON
    // Caminho relativo ao executável
    gameMap = MapLoader_Load("assets/maps/maptest.json");
    
    // 2. Configurar Player
    // Posição inicial hardcoded (idealmente viria do JSON layer objects)
    player.position = (Vector2){ 7, 12 }; 
    player.speed = 0;
    player.canJump = false;
    
    // 3. Configurar Camera
    camera.target = player.position;
    camera.offset = (Vector2){ LARGURA_TELA/2.0f, ALTURA_TELA/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 2.2f; // Zoom maior pois tiles são pequenos (16x16)
}

EstadoJogo Gameplay_Update(void) {
    float dt = GetFrameTime();

    if (IsKeyPressed(KEY_ESCAPE)) return TELA_MENU;

    // Lógica
    Entities_ProcessPlayerInput(&player, dt);
    Physics_UpdatePlayer(&player, &gameMap, dt); // Passamos o Mapa, não EnvItems
    Render_UpdateCamera(&camera, &player, &gameMap, LARGURA_TELA, ALTURA_TELA);

    return TELA_GAMEPLAY;
}

void Gameplay_Draw(void) {
    ClearBackground(LIGHTGRAY);
    
    ClearBackground(SKYBLUE); // Cor de fundo básica
    
    BeginMode2D(camera);
        Render_Map(&gameMap);    // Renderiza Tiles
        Render_Player(&player);  // Renderiza Player
    EndMode2D();
    
    DrawText("Controles: Setas + Espaco | ESC para fechar", 20, 20, 20, BLACK);
}

void Gameplay_Deinit(void) {
    MapLoader_Unload(&gameMap);
}