#include "scene_manager.h"
#include "../systems/systems.h"
#include "../ui/ui.h"
#include "../config/config.h"

// Removemos a Texture2D, não precisamos mais dela.

static bool gameoverinicializado = false;

// BOTÕES
// Ajustei as posições Y para ficarem abaixo do texto
static BotaoUI btnMenu = {{LARGURA_TELA/2 - 100, 400, 200, 50}, "MENU", false};
static BotaoUI btnCreditos = {{LARGURA_TELA/2 - 100, 500, 200, 50}, "CRÉDITOS", false};

void GameOver_Init(void){
    // Como tiramos a imagem, o Init fica praticamente vazio, 
    // mas mantemos a flag para consistência do sistema.
    if (gameoverinicializado) return;
    gameoverinicializado = true;
}

EstadoJogo GameOver_Update(void){
    Vector2 mouse = GetMousePosition();

    // Atualiza estado dos botões
    btnMenu.estaSelecionado = CheckCollisionPointRec(mouse, btnMenu.retangulo);
    btnCreditos.estaSelecionado = CheckCollisionPointRec(mouse, btnCreditos.retangulo);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        
        // Se clicar em MENU, volta pra tela inicial
        // (Lá no menu, o botão Jogar já tem o 'Reset' que criamos antes)
        if (btnMenu.estaSelecionado) {
            return TELA_MENU;
        }

        if (btnCreditos.estaSelecionado) {
            return TELA_CREDITOS;
        }
    }
    return TELA_GAMEOVER;
}

void GameOver_Draw(void) {
    // 1. Fundo Preto (Fica melhor para Game Over)
    ClearBackground(BLACK);

    // 2. TEXTO CENTRALIZADO "GAME OVER"
    const char* texto = "GAME OVER";
    int tamanhoFonte = 80; // Fonte bem grande
    Color corTexto = RED;  // Vermelho sangue
    
    // Mede o tamanho do texto para centralizar
    int larguraTexto = MeasureText(texto, tamanhoFonte);
    int posX = (LARGURA_TELA - larguraTexto) / 2;
    int posY = (ALTURA_TELA / 2) - 150; // Joga um pouco pra cima do centro

    DrawText(texto, posX, posY, tamanhoFonte, corTexto);

    // 3. TEXTO SECUNDÁRIO (Opcional)
    const char* subTexto = "Voce Morreu!";
    int tamSub = 30;
    int largSub = MeasureText(subTexto, tamSub);
    DrawText(subTexto, (LARGURA_TELA - largSub)/2, posY + 90, tamSub, DARKGRAY);

    // 4. BOTÕES
    UI_DesenharBotao(&btnMenu);
    UI_DesenharBotao(&btnCreditos);
}

void GameOver_Deinit(void) {
    if(gameoverinicializado) {
        // Nada de textura para descarregar aqui
        gameoverinicializado = false;
    }
}