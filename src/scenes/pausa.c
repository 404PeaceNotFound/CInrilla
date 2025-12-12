#include "scene_manager.h"
#include "../systems/systems.h"
#include "../ui/ui.h"
#include "../config/config.h"

//static AnimacaoSpritesheet animPausa;
static Texture2D fundoPausa;
static bool fundoCapturado = false;
static bool pausainicializado = false;
static BotaoUI btnRetornar = {{LARGURA_TELA/2 - 100, 300, 200, 50}, "RETORNAR", false};
static BotaoUI btnCreditos = {{LARGURA_TELA/2 - 100, 400, 200, 50}, "CRÉDITOS", false};

void Pausa_Init(void){
    if (pausainicializado) return;
    // Image captura = LoadImageFromScreen();
    //fundoPausa = LoadTextureFromImage(captura);
    //UnloadImage(captura);
    //animPausa = Render_CreateAnim("assets/sprites/menu/menu_spritesheet.png", 11, 11, 30, LARGURA_TELA, ALTURA_TELA);
    pausainicializado = true;
}

void Pausa_CapturaFundo(void){
    if(fundoCapturado){
        UnloadTexture(fundoPausa);
    }
    Image captura = LoadImageFromScreen();
    fundoPausa = LoadTextureFromImage(captura);
    UnloadImage(captura);
    fundoCapturado = true;
}

EstadoJogo Pausa_Update(void){
    Vector2 mouse = GetMousePosition();
    //Render_UpdateAnim(&animPausa, GetFrameTime());

    btnRetornar.estaSelecionado = CheckCollisionPointRec(mouse, btnRetornar.retangulo);
    btnCreditos.estaSelecionado = CheckCollisionPointRec(mouse, btnCreditos.retangulo);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (btnRetornar.estaSelecionado) return TELA_GAMEPLAY;
        if (btnCreditos.estaSelecionado) return TELA_CREDITOS;
    }
    return TELA_PAUSA;
}

void Pausa_Draw(void) {
    //ClearBackground(RAYWHITE);
    if (fundoCapturado){
    DrawTexture(fundoPausa, 0, 0, WHITE);
    }
    //Render_DrawAnim(animPausa, (Vector2){LARGURA_TELA/2.0f, ALTURA_TELA/2.0f});
    DrawRectangle(0, 0, LARGURA_TELA, ALTURA_TELA, (Color){0, 0, 0, 150});

    UI_DesenharTextoCentralizado(TITULO_JOGO, 100, 60, WHITE);
    UI_DesenharBotao(&btnRetornar);
    UI_DesenharBotao(&btnCreditos);
}

void Pausa_Deinit(void) {
    if(pausainicializado) {
        if (fundoCapturado) {
            UnloadTexture(fundoPausa);
            fundoCapturado = false;
        }
        //Render_UnloadAnim(&animPausa);
        pausainicializado = false;
    }
}

