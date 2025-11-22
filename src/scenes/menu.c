#include "scene_manager.h"
#include "../systems/systems.h"
#include "../ui/ui.h"
#include "../config/config.h"

static AnimacaoSpritesheet animMenu;
static bool menuinicializado = false;
static BotaoUI btnJogar = {{LARGURA_TELA/2 - 100, 300, 200, 50}, "JOGAR", false};
static BotaoUI btnCreditos = {{LARGURA_TELA/2 - 100, 400, 200, 50}, "CREDITOS", false};

void Menu_Init(void) {
    if (menuinicializado) return;
    animMenu = Render_CreateAnim("assets/sprites_menu/menu_spritesheet.png", 11, 11, 30, LARGURA_TELA, ALTURA_TELA);
    menuinicializado = true;
}

EstadoJogo Menu_Update(void) {
    Vector2 mouse = GetMousePosition();
    Render_UpdateAnim(&animMenu, GetFrameTime());
    
    btnJogar.estaSelecionado = CheckCollisionPointRec(mouse, btnJogar.retangulo);
    btnCreditos.estaSelecionado = CheckCollisionPointRec(mouse, btnCreditos.retangulo);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (btnJogar.estaSelecionado) return TELA_GAMEPLAY;
        if (btnCreditos.estaSelecionado) return TELA_CREDITOS;
    }
    return TELA_MENU;
}

void Menu_Draw(void) {
    ClearBackground(RAYWHITE);
    Render_DrawAnim(animMenu, (Vector2){LARGURA_TELA/2.0f, ALTURA_TELA/2.0f});


    UI_DesenharTextoCentralizado(TITULO_JOGO, 100, 60, WHITE);
    UI_DesenharBotao(&btnJogar);
    UI_DesenharBotao(&btnCreditos);
}

void Menu_Deinit(void) {
    if(menuinicializado) {
        Render_UnloadAnim(&animMenu);
        menuinicializado = false;
    }
}