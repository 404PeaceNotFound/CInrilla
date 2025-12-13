#include "scene_manager.h"
#include "../systems/systems.h"
#include "../ui/ui.h"
#include "../config/config.h"

// Removemos a Texture2D, pois será puramente código/texto.

static bool winnerinicializado = false;

// BOTÕES
// Posicionados centralizados horizontalmente
static BotaoUI btnMenu = {{LARGURA_TELA/2 - 100, 400, 200, 50}, "MENU", false};
static BotaoUI btnCreditos = {{LARGURA_TELA/2 - 100, 500, 200, 50}, "CRÉDITOS", false};

void Winner_Init(void){
    if (winnerinicializado) return;
    winnerinicializado = true;
}

EstadoJogo Winner_Update(void){
    Vector2 mouse = GetMousePosition();

    // Atualiza colisão do mouse com os botões
    btnMenu.estaSelecionado = CheckCollisionPointRec(mouse, btnMenu.retangulo);
    btnCreditos.estaSelecionado = CheckCollisionPointRec(mouse, btnCreditos.retangulo);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        
        // Volta para o Menu (onde o botão Jogar reseta o jogo)
        if (btnMenu.estaSelecionado) {
            return TELA_MENU;
        }
        
        if (btnCreditos.estaSelecionado) {
            return TELA_CREDITOS;
        }
    }
    return TELA_WINNER;
}

void Winner_Draw(void) {
    // 1. Fundo
    ClearBackground(BLACK); // Fundo preto destaca bem o verde/dourado

    // 2. TÍTULO "VITÓRIA!"
    const char* texto = "VITORIA!";
    int tamanhoFonte = 80;
    Color corTexto = GREEN; // Verde para sucesso (ou use GOLD se preferir)
    
    // Centralização Matemática
    int larguraTexto = MeasureText(texto, tamanhoFonte);
    int posX = (LARGURA_TELA - larguraTexto) / 2;
    int posY = (ALTURA_TELA / 2) - 150; 

    DrawText(texto, posX, posY, tamanhoFonte, corTexto);

    // 3. SUBTÍTULO
    const char* subTexto = "Voce completou a jornada!";
    int tamSub = 30;
    int largSub = MeasureText(subTexto, tamSub);
    DrawText(subTexto, (LARGURA_TELA - largSub)/2, posY + 90, tamSub, LIGHTGRAY);

    // 4. DESENHA OS BOTÕES
    UI_DesenharBotao(&btnMenu);
    UI_DesenharBotao(&btnCreditos);
}

void Winner_Deinit(void) {
    if(winnerinicializado) {
        winnerinicializado = false;
    }
}