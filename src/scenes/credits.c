#include "scene_manager.h"
#include "../systems/systems.h"
#include "../ui/ui.h"
#include "../config/config.h"

static AnimacaoSpritesheet animFundo;
static bool inicializado = false;
static BotaoUI btnVoltar;

void Creditos_Init(void) {
    if (inicializado) return;
    animFundo = Render_CreateAnim("assets/sprites/creditos/sheet_1.png", 6, 11, 30, LARGURA_TELA, ALTURA_TELA, true);
    btnVoltar = (BotaoUI){{LARGURA_TELA - 280, ALTURA_TELA - 80, 200, 50}, "VOLTAR", false};
    inicializado = true;
}

EstadoJogo Creditos_Update(void) {
    Render_UpdateAnim(&animFundo, GetFrameTime());
    
    btnVoltar.estaSelecionado = CheckCollisionPointRec(GetMousePosition(), btnVoltar.retangulo);
    if (btnVoltar.estaSelecionado && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) return TELA_MENU;
    
    return TELA_CREDITOS;
}

void Creditos_Draw(void) {
    ClearBackground(BLACK);
    Render_DrawAnim(animFundo, (Vector2){LARGURA_TELA/2.0f, ALTURA_TELA/2.0f});
    
    UI_DesenharTextoCentralizado("CREDITOS", 50, 100, WHITE);
    UI_DesenharTextoCentralizado("Gabriel Vieira <g>", ALTURA_TELA/2.0f - 100, 50, WHITE);
    UI_DesenharTextoCentralizado("Guilherme Vitor <g>", ALTURA_TELA/2.0f - 50, 50, WHITE);
    UI_DesenharTextoCentralizado("João <j>", ALTURA_TELA/2.0f, 50, WHITE);
    UI_DesenharTextoCentralizado("José <j>", ALTURA_TELA/2.0f + 50, 50, WHITE);
    UI_DesenharTextoCentralizado("Luciano <l>", ALTURA_TELA/2.0f + 100, 50, WHITE);
    UI_DesenharBotao(&btnVoltar);
}

void Creditos_Deinit(void) {
    if (inicializado) {
        Render_UnloadAnim(&animFundo);
        inicializado = false;
    }
}