#ifndef MENU_H
#define MENU_H

#include "raylib.h"
#include "game_logic.h"  // Agora inclui game.h para conhecer GameState

typedef enum {
    BUTTON_START,
    BUTTON_CREDITS,
    BUTTON_QUIT
} ButtonType;

typedef struct {
    Rectangle bounds;
    const char* text;
    ButtonType type;
    bool isHovered;
} Button;

void InitMenu(void);
GameState UpdateMenu(void);  // Agora retorna GameState corretamente
void DrawMenu(void);
void CloseMenu(void);
void DrawButton(Button* button);

#endif