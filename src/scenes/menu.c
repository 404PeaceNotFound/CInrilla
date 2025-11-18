#include "menu.h"
#include "../core/window.h"
#include "../core/audio_core.h"
#include "../ui/widgets.h"
#include <raylib.h>
#include <raymath.h>

// Definições específicas da animação do Menu
#define CONTAGEM_FRAME_X 11
#define CONTAGEM_FRAME_Y 11
#define FPS_FRAME 30
#define TEMPO_FRAME (1.0f / FPS_FRAME)

// Variáveis de estado da cena (STATIC)
static Texture2D spriteMenu;
static int indiceFrame_x = 0;
static int indiceFrame_y = 0;
static bool inverterAnimacao = false;
static float temporizadorFrame = TEMPO_FRAME;

// Botões do menu
static Botao btnJogar;
static Botao btnCreditos;
static Botao btnSair;

void InicializarMenu(void) {
    spriteMenu = LoadTexture("assets/sprites_menu/menu_spritesheet.png");
    
    // Inicialização de variáveis de animação
    indiceFrame_x = 0;
    indiceFrame_y = 0;
    inverterAnimacao = false;
    temporizadorFrame = TEMPO_FRAME;

    // Inicialização dos botões
    int larguraBotao = 200;
    int alturaBotao = 50;
    int x = LARGURA_TELA/2 - larguraBotao/2;
    int y = ALTURA_TELA/2 - alturaBotao/2;

    btnJogar = (Botao){ 
        {x, y, larguraBotao, alturaBotao}, 
        "JOGAR", 
        TELA_JOGO, 
        false 
    };

    btnCreditos = (Botao){ 
        {x, y + 60, larguraBotao, alturaBotao}, 
        "CRÉDITOS", 
        TELA_CREDITOS, 
        false 
    };

    btnSair = (Botao){ 
        {x, y + 120, larguraBotao, alturaBotao}, 
        "SAIR", 
        TELA_SAIR, 
        false 
    };
}

EstadoJogo AtualizarMenu(void) {
    temporizadorFrame -= GetFrameTime();

    if (temporizadorFrame < 0) {
        temporizadorFrame = TEMPO_FRAME;

        // Lógica de loop dos frames (mantida da sua versão)
        if (!inverterAnimacao) {
            indiceFrame_x++;
            if (indiceFrame_x >= CONTAGEM_FRAME_X) {
                indiceFrame_x = 0;
                indiceFrame_y++;
            }
        } else {
            indiceFrame_x--;
            if (indiceFrame_x < 0) {
                indiceFrame_x = CONTAGEM_FRAME_X - 1;
                indiceFrame_y--;
            }
        }
        
        // Lógica de inversão da animação
        if (indiceFrame_x == CONTAGEM_FRAME_X - 1 && indiceFrame_y == CONTAGEM_FRAME_Y - 1) {
            inverterAnimacao = true;
        } else if (indiceFrame_x <= 0 && indiceFrame_y <= 0) {
            inverterAnimacao = false;
        }
    }
    
    // LÓGICA DOS BOTÕES - AGORA FUNCIONAL
    Vector2 mousePos = GetMousePosition();
    btnJogar.estaSelecionado = CheckCollisionPointRec(mousePos, btnJogar.retangulo);
    btnCreditos.estaSelecionado = CheckCollisionPointRec(mousePos, btnCreditos.retangulo);
    btnSair.estaSelecionado = CheckCollisionPointRec(mousePos, btnSair.retangulo);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (btnJogar.estaSelecionado) {
            TocarSomUI(); // Toca som de confirmação
            return btnJogar.acao;
        } else if (btnCreditos.estaSelecionado) {
            TocarSomUI();
            return btnCreditos.acao;
        } else if (btnSair.estaSelecionado) {
            TocarSomUI();
            return btnSair.acao;
        }
    }

    // Tecla ESC também sai
    if (IsKeyPressed(KEY_ESCAPE)) {
        return TELA_SAIR;
    }

    return TELA_MENU; 
}

void DesenharMenu(void) {
    ClearBackground(RAYWHITE);
    
    // Cálculo da área de origem na spritesheet
    Rectangle origem = (Rectangle){
        indiceFrame_x * LARGURA_TELA, 
        indiceFrame_y * ALTURA_TELA, 
        LARGURA_TELA, 
        ALTURA_TELA
    };
    
    // Cálculo da área de destino (centralizada)
    Rectangle destino = (Rectangle){
        LARGURA_TELA/2.0f, 
        ALTURA_TELA/2.0f, 
        origem.width, 
        origem.height
    };
    
    // Desenho da animação de fundo
    DrawTexturePro(
        spriteMenu, 
        origem, 
        destino, 
        (Vector2){destino.width/2.0f, destino.height/2.0f},
        0, 
        WHITE
    );

    // Desenho dos botões
    DesenharBotao(&btnJogar);
    DesenharBotao(&btnCreditos);
    DesenharBotao(&btnSair);

    // Texto do título
    DrawText("CINRILLA", LARGURA_TELA/2 - MeasureText("CINRILLA", 60)/2, 100, 60, WHITE);
}

void EncerrarMenu(void) {
    UnloadTexture(spriteMenu);
}