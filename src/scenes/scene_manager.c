#include "scene_manager.h"
#include "../core/window.h"      // Vamos criar esse wrapper simples
#include "../core/audio_core.h"
#include "../scenes/menu.h"
#include "creditos.h"
// #include "gameplay.h" // Futuro

static EstadoJogo estadoAtual = TELA_MENU;

void InicializarGerenciadorCenas(void) {
    InitWindow(1280, 720, "CInrilla");
    SetTargetFPS(60);
    
    InicializarSistemaAudio(); // Core audio
    InicializarMenu();         // Carrega assets do menu
    InicializarCreditos();     // Prepara assets dos creditos
}

void AtualizarGerenciadorCenas(void) {
    switch (estadoAtual) {
        case TELA_MENU:
            // O UpdateMenu retorna o novo estado desejado
            estadoAtual = AtualizarMenu(); 
            break;
        case TELA_JOGO:
            // AtualizarGameplay();
            break;
        case TELA_CREDITOS:
            estadoAtual = AtualizarCreditos();
            break;
        case TELA_SAIR:
            break;
    }
}

void DesenharGerenciadorCenas(void) {
    BeginDrawing();
    
    switch (estadoAtual) {
        case TELA_MENU:
            DesenharMenu();
            break;
        case TELA_JOGO:
            ClearBackground(BLACK);
            DrawText("JOGO EM DESENVOLVIMENTO", 400, 360, 20, WHITE);
            break;
        case TELA_CREDITOS:
            DesenharCreditos();
            break;
        default: break;
    }
    
    EndDrawing();
}

void EncerrarGerenciadorCenas(void) {
    EncerrarMenu();
    EncerrarCreditos();
    EncerrarSistemaAudio();
    CloseWindow();
}

bool DeveFecharJogo(void) {
    return WindowShouldClose() || estadoAtual == TELA_SAIR;
}

void MudarEstado(EstadoJogo novoEstado) {
    estadoAtual = novoEstado;
}