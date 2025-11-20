#include "creditos.h"
#include "../scenes/scene_manager.h"
#include "../core/window.h"
#include "../ui/widgets.h"
#include "../utils/spritesheet_animacao.h" // NOVO
#include <raylib.h>
#include <stdbool.h>

// --- Definições específicas da Animação de Créditos ---
#define CREDITOS_FRAME_X 6
#define CREDITOS_FRAME_Y 11
#define CREDITOS_FPS 30

// Variáveis de Estado da Cena
static AnimacaoSpritesheet animacaoFundoCreditos;
static Botao btnVoltar;
static bool inicializado = false;

void InicializarCreditos(void) {
    if (inicializado) return;

    // Inicialização da Animação (Chamada ao módulo utilitário)
    animacaoFundoCreditos = CriarAnimacaoSpritesheet(
        "assets/sprites_creditos/sheet_1.png", 
        CREDITOS_FRAME_X, 
        CREDITOS_FRAME_Y, 
        CREDITOS_FPS, 
        LARGURA_TELA, 
        ALTURA_TELA
    );
    
    // Inicialização do Botão Voltar
    btnVoltar = (Botao){ 
        {LARGURA_TELA - 280, ALTURA_TELA - 80, 200, 50}, 
        "VOLTAR AO MENU", 
        TELA_MENU, 
        false 
    };
    
    inicializado = true;
}

EstadoJogo AtualizarCreditos(void) {
    // Lógica da Animação (SIMPLES, usando o módulo)
    AtualizarAnimacaoSpritesheet(&animacaoFundoCreditos, GetFrameTime()); 

    // Lógica do Botão Voltar (Input)
    btnVoltar.estaSelecionado = CheckCollisionPointRec(GetMousePosition(), btnVoltar.retangulo);
    
    if (btnVoltar.estaSelecionado && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        return TELA_MENU; 
    }
    if(IsKeyPressed(KEY_ESCAPE)) return TELA_MENU;

    return TELA_CREDITOS;
}

void DesenharCreditos(void) {
    ClearBackground(BLACK);
    
    // Desenho da Animação (SIMPLES, usando o módulo)
    DesenharAnimacaoSpritesheet(animacaoFundoCreditos, (Vector2){LARGURA_TELA/2.0f, ALTURA_TELA/2.0f});
    
    // Desenho do Conteúdo de Créditos
    DrawText("CRÉDITOS", LARGURA_TELA/2 - MeasureText("CRÉDITOS", 50)/2, 50, 50, WHITE);
    DrawText("Equipe:", 240, 200, 30, WHITE);
    DrawText("Gabriel Vieira <gvnrb>", 240, 250, 30, WHITE);
    DrawText("Guilherme Vitor <gvca>", 240, 300, 30, WHITE);
    DrawText("Joao Victor <jv>", 240, 350, 30, WHITE);
    DrawText("Jose <j>", 240, 400, 30, WHITE);
    DrawText("Luciano <l>", 240, 450, 30, WHITE);

    
    // Desenho do Botão (Reutilizando UI/Widgets)
    DesenharBotao(&btnVoltar);
}

void EncerrarCreditos(void) {
    if (inicializado) {
        EncerrarAnimacaoSpritesheet(&animacaoFundoCreditos);
        inicializado = false;
    }
}