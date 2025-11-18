#include "menu.h"
#include "audio.h"
#include "scene_manager.h"
#include "creditos.h"  // Para TelaCreditos()
#include <stdio.h>     // Para printf()

static Button buttons[3];
static bool mouseFlag = false;

void InitMenu(void)
{
    // Configurar botões
    buttons[BUTTON_START] = (Button){ {-5, 300, 300, 60}, "Iniciar Jogo", BUTTON_START, false };
    buttons[BUTTON_CREDITS] = (Button){ {-5, 400, 300, 60}, "Créditos", BUTTON_CREDITS, false };
    buttons[BUTTON_QUIT] = (Button){ {-5, 500, 300, 60}, "Sair", BUTTON_QUIT, false };
}

GameState UpdateMenu(void)
{
    mouseFlag = false;
    
    for (int i = 0; i < 3; i++) {
        buttons[i].isHovered = CheckCollisionPointRec(GetMousePosition(), buttons[i].bounds);
        
        if (buttons[i].isHovered) {
            if (!mouseFlag) {
                PlayUISound();
                mouseFlag = true;
            }
            
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                switch (buttons[i].type) {
                    case BUTTON_START:
                        printf("Botão JOGAR foi clicado\n");
                        return SCREEN_GAME;
                    case BUTTON_CREDITS:
                        TelaCreditos();
                        return SCREEN_CREDITS;
                    case BUTTON_QUIT:
                        return SCREEN_EXIT;
                }
            }
        }
    }
    
    return SCREEN_MENU;
}

void DrawMenu(void) {
    DrawMenuBackground();
    
    // Título
    DrawText("CInrilla:", 264, 104, 55, BLACK);
    DrawText("CInrilla:", 260, 100, 55, WHITE);
    
    // Botões
    for (int i = 0; i < 3; i++) {
        DrawButton(&buttons[i]);
    }
    
    // Footer
    DrawFooter("CIN-UFPE 2025");
}

void CloseMenu(void) {
    // Limpar recursos do menu se necessário
}