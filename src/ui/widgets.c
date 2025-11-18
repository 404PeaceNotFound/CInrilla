#include "widgets.h"

void DesenharBotao(Botao* botao) {
    Color corBotao = botao->estaSelecionado ? 
        (Color){ 50, 50, 50, 255 } : 
        (Color){ 30, 30, 30, 150 };
    
    DrawRectangleRec(botao->retangulo, corBotao);
    
    const int fontSize = 20;
    int larguraTexto = MeasureText(botao->texto, fontSize);
    int x = botao->retangulo.x + (botao->retangulo.width - larguraTexto) / 2;
    int y = botao->retangulo.y + (botao->retangulo.height - fontSize) / 2;
    
    DrawText(botao->texto, x, y, fontSize, RAYWHITE);
}

void DesenharRodape(const char* texto) {
    const int fontSize = 20;
    int larguraTexto = MeasureText(texto, fontSize);
    int x = (1280 / 2) - (larguraTexto / 2);
    int y = 700;
    DrawText(texto, x, y, fontSize, WHITE);
}