#include "ui.h"
#include "../config/config.h"
#include <raylib.h>
#include <math.h> // Necessário para fmaxf, fminf se a Raylib não tiver o Clamp

void UI_DesenharBotao(BotaoUI* botao) {
    Color corBotao = botao->estaSelecionado ? 
        (Color){ 50, 50, 50, 255 } : 
        (Color){ 30, 30, 30, 150 };
    
    DrawRectangleRec(botao->retangulo, corBotao);
    
    int larguraTexto = MeasureText(botao->texto, 20);
    int x = botao->retangulo.x + (botao->retangulo.width - larguraTexto) / 2;
    int y = botao->retangulo.y + (botao->retangulo.height - 20) / 2;
    
    DrawText(botao->texto, x, y, 20, RAYWHITE);
}

void UI_DesenharTextoCentralizado(const char* texto, int y, int tamanho, Color cor) {
    int largura = MeasureText(texto, tamanho);
    DrawText(texto, (GetScreenWidth() - largura) / 2, y, tamanho, cor);
}

void UI_DesenharHealthBar(int HealthAtual, int HealthMax, int screenWidth) {
    // Dimensões fixas para a barra
    const int BarTamanhoMax = 200;
    const int BarAltura = 20;
    const int BarX = 50;
    const int BarY = 50;

    // Calcula a largura atual da barra com base na saúde
    // Variável HealthBarWidth é agora usada para armazenar o cálculo e desenhar
    float HealthBarWidth = ((float)HealthAtual / HealthMax) * BarTamanhoMax;

    // Garante que a barra não seja negativa
    HealthBarWidth = fmaxf(0, HealthBarWidth); 
    
    // Fundo da barra (Container)
    DrawRectangle(BarX, BarY, BarTamanhoMax, BarAltura, GRAY); 
    
    // Barra de Vida
    DrawRectangle(BarX, BarY, (int)HealthBarWidth, BarAltura, RED);

    // Borda da barra
    DrawRectangleLines(BarX, BarY, BarTamanhoMax, BarAltura, BLACK);

    // Texto (Exemplo: "10/15")
    DrawText(TextFormat("%d / %d", HealthAtual, HealthMax), BarX + BarTamanhoMax + 10, BarY, 20, WHITE);
}