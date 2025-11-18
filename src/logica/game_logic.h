#ifndef GAME_H
#define GAME_H

#include "raylib.h"

typedef enum {
    SCREEN_MENU,
    SCREEN_GAME,
    SCREEN_CREDITS,
    SCREEN_EXIT
} GameState;

// Funções principais do jogo
void InitGame(void);
void UpdateGame(void);
void DrawGame(void);
void CloseGame(void);
bool ShouldCloseGame(void);

// Getter para o estado atual (para outros módulos)
GameState GetCurrentGameState(void);

#endif