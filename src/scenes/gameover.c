#include "scene_manager.h"
#include "../systems/systems.h"
#include "../ui/ui.h"
#include "../config/config.h"

//static AnimacaoSpritesheet animPausa;
static bool gameoverinicializado = false;
//static BotaoUI btnRetornar = {{LARGURA_TELA/2 - 100, 300, 200, 50}, "RETORNAR", false};
static BotaoUI btnCreditos = {{LARGURA_TELA/2 - 100, 600, 200, 50}, "CRÉDITOS", false};
static Texture2D fundoGameOver;

void GameOver_Init(void){
    if (gameoverinicializado) return;
    fundoGameOver = LoadTexture("assets/images/Fundo_GameOver.png");
    //animGameOver = Render_CreateAnim("assets/sprites/menu/menu_spritesheet.png", 11, 11, 30, LARGURA_TELA, ALTURA_TELA);
    //gameoverinicializado = true;
}

EstadoJogo GameOver_Update(void){
    Vector2 mouse = GetMousePosition();
    //Render_UpdateAnim(&animGameOver, GetFrameTime());

    //btnRetornar.estaSelecionado = CheckCollisionPointRec(mouse, btnRetornar.retangulo);
    btnCreditos.estaSelecionado = CheckCollisionPointRec(mouse, btnCreditos.retangulo);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        //if (btnRetornar.estaSelecionado) return TELA_GAMEPLAY;
        if (btnCreditos.estaSelecionado) return TELA_CREDITOS;
    }
    return TELA_GAMEOVER;
}

void GameOver_Draw(void) {
    ClearBackground(RAYWHITE);
    //Render_DrawAnim(animGameOver, (Vector2){LARGURA_TELA/2.0f, ALTURA_TELA/2.0f});
    DrawTexture(fundoGameOver, 0, 0, WHITE);

    //UI_DesenharTextoCentralizado(TITULO_JOGO, 100, 60, WHITE);
    //UI_DesenharBotao(&btnRetornar);
    UI_DesenharBotao(&btnCreditos);
}

void GameOver_Deinit(void) {
    if(gameoverinicializado) {
        //Render_UnloadAnim(&animGameOver);
        UnloadTexture(fundoGameOver);
        gameoverinicializado = false;
    }
}

