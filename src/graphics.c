#include "graphics.h"
#include "menu.h"  // Agora inclui menu.h para a definição completa de Button
#include <stdio.h> 

#define TOTAL_FRAMES 106

static Texture2D menuFrames[TOTAL_FRAMES + 1];
static int currentFrame = 0;
static bool reverseAnimation = false;

void InitGraphics(void) {
    // Carregar frames da animação
    for (int i = 0; i <= TOTAL_FRAMES; i++) {
        char path[64];
        sprintf(path, "assets/frames_menu/frame_%03d.png", i + 1);
        menuFrames[i] = LoadTexture(path);
    }
}

void DrawMenuBackground(void) {
    // Atualizar animação
    if (!reverseAnimation) {
        currentFrame++;
        if (currentFrame >= TOTAL_FRAMES) reverseAnimation = true;
    } else {
        currentFrame--;
        if (currentFrame == 0) reverseAnimation = false;
    }
    
    DrawTexture(menuFrames[currentFrame], 0, 0, WHITE);
}

void DrawButton(Button* button) {
    Color buttonColor = button->isHovered ? 
        (Color){ 50, 50, 50, 255 } : 
        (Color){ 30, 30, 30, 150 };
    
    DrawRectangleRec(button->bounds, buttonColor);
    
    // Centralizar texto
    const int fontSize = 20;
    int textWidth = MeasureText(button->text, fontSize);
    int x = button->bounds.x + (button->bounds.width - textWidth) / 2;
    int y = button->bounds.y + (button->bounds.height - fontSize) / 2;
    
    DrawText(button->text, x, y, fontSize, RAYWHITE);
}

void DrawFooter(const char* text) {
    const int fontSize = 20;
    int textWidth = MeasureText(text, fontSize);
    int x = (1280 / 2) - (textWidth / 2);
    int y = 700;
    
    DrawText(text, x, y, fontSize, WHITE);
}

void CloseGraphics(void) {
    for (int i = 0; i <= TOTAL_FRAMES; i++) {
        UnloadTexture(menuFrames[i]);
    }
}