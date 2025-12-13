#include "scene_manager.h"
#include <raylib.h>
#include "../core/core_platform.h"
#define LARGURA_TELA 1280
#define ALTURA_TELA 720

static EstadoJogo estadoAtual = TELA_MENU;

// ============================================================================
// FUNÇÕES LOCAIS DA TELA DE TRANSIÇÃO
// (Pode colocar aqui mesmo para facilitar)
// ============================================================================
EstadoJogo Transicao_Update(void) {
    // Espera o jogador apertar ENTER para continuar
    if (IsKeyPressed(KEY_ENTER)) {
        return TELA_GAMEPLAY; 
    }
    return TELA_TRANSICAO;
}

void Transicao_Draw(void) {
    ClearBackground(BLACK); 

    // --- 1. TÍTULO ("FASE CONCLUÍDA!") ---
    const char* textoTitulo = "FASE CONCLUIDA!";
    int tamanhoFonteTitulo = 40;
    
    // Mede a largura do texto em pixels
    int larguraTextoTitulo = MeasureText(textoTitulo, tamanhoFonteTitulo);
    
    // Calcula a posição X para ficar no centro
    int posX_Titulo = (LARGURA_TELA - larguraTextoTitulo) / 2;
    int posY_Titulo = (ALTURA_TELA / 2) - 50; // Um pouco acima do meio


    // --- 2. SUBTÍTULO ("Pressione ENTER...") ---
    const char* textoSub = "Pressione ENTER para a proxima fase";
    int tamanhoFonteSub = 20;
    
    int larguraTextoSub = MeasureText(textoSub, tamanhoFonteSub);
    
    int posX_Sub = (LARGURA_TELA - larguraTextoSub) / 2;
    int posY_Sub = (ALTURA_TELA / 2) + 10; // Um pouco abaixo do meio


    // --- 3. DESENHA ---
    DrawText(textoTitulo, posX_Titulo, posY_Titulo, tamanhoFonteTitulo, GREEN);
    DrawText(textoSub, posX_Sub, posY_Sub, tamanhoFonteSub, WHITE);
}
// ============================================================================


void SM_Init(void) {
    Menu_Init();
    Gameplay_Init();
    Creditos_Init();
    Pausa_Init();
    Core_PlayMusic();
    GameOver_Init();
    Winner_Init();
}

void SM_Update(void) {

    EstadoJogo proximoEstado = estadoAtual;

    switch (estadoAtual) {
        case TELA_MENU: proximoEstado = Menu_Update(); break;
        case TELA_GAMEPLAY: proximoEstado = Gameplay_Update(); break;
        
        // --- ADICIONE ISTO ---
        case TELA_TRANSICAO: proximoEstado = Transicao_Update(); break;
        // ---------------------
        
        case TELA_CREDITOS: proximoEstado = Creditos_Update(); break;
        case TELA_PAUSA: proximoEstado = Pausa_Update(); break;
        case TELA_GAMEOVER: proximoEstado = GameOver_Update(); break;
        case TELA_WINNER: proximoEstado = Winner_Update(); break;
        default: break;
    }

    // --- LÓGICA DE MUDANÇA DE ESTADO ---
    if (proximoEstado != estadoAtual){
        
        if (proximoEstado == TELA_PAUSA){
            Pausa_CapturaFundo();
        }

        // SE O JOGADOR APERTOU ENTER NA TELA DE TRANSIÇÃO:
        if (estadoAtual == TELA_TRANSICAO && proximoEstado == TELA_GAMEPLAY) {
            Gameplay_ProximoNivel(); // <--- Carrega a Fase 2!
        }

        // SE O JOGADOR REINICIOU NO GAMEOVER:
        if (estadoAtual == TELA_GAMEOVER && proximoEstado == TELA_GAMEPLAY) {
            Gameplay_Reiniciar();
        }
    }

    if (proximoEstado == TELA_SAIR) {
        CloseWindow(); 
    }
    estadoAtual = proximoEstado;
}

void SM_Draw(void){
    BeginDrawing();
    
    // O ClearBackground(RAYWHITE) padrão pode ser movido para dentro dos cases
    // ou mantido aqui. Mas a Transição vai pintar de Preto por cima.
    ClearBackground(RAYWHITE);

    switch (estadoAtual) {
        case TELA_MENU: Menu_Draw(); break;
        case TELA_GAMEPLAY: Gameplay_Draw(); break;
        
        // --- ADICIONE ISTO ---
        case TELA_TRANSICAO: Transicao_Draw(); break;
        // ---------------------
        
        case TELA_CREDITOS: Creditos_Draw(); break;
        case TELA_PAUSA: Pausa_Draw(); break;
        case TELA_GAMEOVER: GameOver_Draw(); break;
        case TELA_WINNER: Winner_Draw(); break;
        default: break;
    }

    EndDrawing();
}

void SM_Deinit(void) {
    Pausa_Deinit();
    Creditos_Deinit();
    // Outros deinits...
}