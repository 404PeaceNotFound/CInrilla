#include "scene_manager.h"
#include <raylib.h>

#include "systems/enemy_system.h"   

static EstadoJogo estadoAtual = TELA_MENU;

void SM_Init(void) {
    Menu_Init();
    Gameplay_Init();
    Creditos_Init();
    EnemySystem_Init(); // geração de inimigos 
}

void SM_Update(void) {

    float dt  = GetFrameTime(); // atualizar inimigos
    EnemySystem_Update(dt);

    EstadoJogo proximoEstado = estadoAtual;

    switch (estadoAtual) {
        case TELA_MENU: proximoEstado = Menu_Update(); break;
        case TELA_GAMEPLAY: proximoEstado = Gameplay_Update(); break;
        case TELA_CREDITOS: proximoEstado = Creditos_Update(); break;
        default: break;
    }

    if (proximoEstado == TELA_SAIR) {
        CloseWindow(); // Força saída
    }
    estadoAtual = proximoEstado;
}

void SM_Draw(void){
    BeginDrawing();
    ClearBackground(RAYWHITE);

    switch (estadoAtual) {
        case TELA_MENU: Menu_Draw(); break;
        case TELA_GAMEPLAY: Gameplay_Draw(); break;
        case TELA_CREDITOS: Creditos_Draw(); break;
        default: break;
    }
    EnemySystem_Draw();

    EndDrawing();
}

void SM_Deinit(void) {
    Creditos_Deinit();
    // Outros deinits se necessário
}