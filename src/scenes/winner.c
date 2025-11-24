#include "scene_manager.h"
#include "../systems/systems.h"
#include "../ui/ui.h"
#include "../config/config.h"

//static AnimacaoSpritesheet animWinner;
static bool winnerinicializado = false;
//static BotaoUI btnRetornar = {{LARGURA_TELA/2 - 100, 300, 200, 50}, "RETORNAR", false};
static BotaoUI btnCreditos = {{LARGURA_TELA/2 - 100, 600, 200, 50}, "CRÉDITOS", false};
static Texture2D fundoWinner;

void Winner_Init(void){
    if (winnerinicializado) return;
    fundoWinner = LoadTexture("assets/images/Fundo_Winner.png");
    //animWinner = Render_CreateAnim("assets/sprites/menu/menu_spritesheet.png", 11, 11, 30, LARGURA_TELA, ALTURA_TELA);
    //winnerinicializado = true;
}

EstadoJogo Winner_Update(void){
    Vector2 mouse = GetMousePosition();
    //Render_UpdateAnim(&animWinner, GetFrameTime());

    //btnRetornar.estaSelecionado = CheckCollisionPointRec(mouse, btnRetornar.retangulo);
    btnCreditos.estaSelecionado = CheckCollisionPointRec(mouse, btnCreditos.retangulo);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        //if (btnRetornar.estaSelecionado) return TELA_GAMEPLAY;
        if (btnCreditos.estaSelecionado) return TELA_CREDITOS;
    }
    return TELA_WINNER;
}

void Winner_Draw(void) {
    ClearBackground(RAYWHITE);
    //Render_DrawAnim(animWinner, (Vector2){LARGURA_TELA/2.0f, ALTURA_TELA/2.0f});
    DrawTexture(fundoWinner, 0, 0, WHITE);

    //UI_DesenharTextoCentralizado(TITULO_JOGO, 100, 60, WHITE);
    //UI_DesenharBotao(&btnRetornar);
    UI_DesenharBotao(&btnCreditos);
}

void Winner_Deinit(void) {
    if(winnerinicializado) {
        //Render_UnloadAnim(&animWinner);
        UnloadTexture(fundoWinner);
        winnerinicializado = false;
    }
}

