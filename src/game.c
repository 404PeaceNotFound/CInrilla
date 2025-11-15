#include "game.h"
#include "menu.h"
#include "audio.h"
#include "graphics.h"

static GameState currentState = SCREEN_MENU;

void InitGame(void) {
    InitWindow(1280, 720, "Princesa Não Encontrada");
    SetTargetFPS(60);
    InitAudioSystem();
    InitMenu();
    InitGraphics();
}

void UpdateGame(void) {
    switch (currentState) {
        case SCREEN_MENU:
            currentState = UpdateMenu();
            break;
        case SCREEN_GAME:
            // Lógica do jogo principal
            break;
        case SCREEN_CREDITS:
            // Lógica dos créditos
            break;
        case SCREEN_EXIT:
            break;
    }
}

void DrawGame(void) {
    BeginDrawing();
    
    switch (currentState) {
        case SCREEN_MENU:
            DrawMenu();
            break;
        case SCREEN_GAME:
            // Desenhar jogo principal
            ClearBackground(BLACK);
            DrawText("TELA DO JOGO - EM DESENVOLVIMENTO", 400, 360, 20, WHITE);
            break;
        case SCREEN_CREDITS:
            // Desenhar créditos
            ClearBackground(BLUE);
            DrawText("TELA DE CRÉDITOS", 400, 360, 20, WHITE);
            break;
        case SCREEN_EXIT:
            break;
    }
    
    EndDrawing();
}

void CloseGame(void) {
    CloseMenu();
    CloseAudioSystem();
    CloseGraphics();
    CloseWindow();
}

bool ShouldCloseGame(void) {
    return WindowShouldClose() || currentState == SCREEN_EXIT;
}

GameState GetCurrentGameState(void) {
    return currentState;
}