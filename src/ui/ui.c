#include "ui.h"

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

void UI_DesenharHealthBar(int HealthAtual, int HealthMax ,int Largura_Tela){
    
    const int BarTamanhoMax = 200;
    const int BarAltura = 20;

    float BarTamanho =((float)HealthAtual / HealthMax) * BarTamanhoMax;
    int x = Largura_Tela - BarTamanhoMax - 20;
    int y = 20;

    DrawRectangle(x,y,BarTamanhoMax,BarAltura,GRAY);

    Color barCor = GREEN;
    if (HealthAtual< HealthMax/4) barCor = RED;
    else if (HealthAtual < HealthAtual/2) barCor = ORANGE;
    else barCor = GREEN;

    DrawRectangle(x, y,(int)BarTamanhoMax,BarAltura, barCor);
    DrawRectangleLines(x, y, BarTamanhoMax, BarAltura, GRAY);
    DrawText(TextFormat("%d / %d", HealthAtual, HealthMax), x, y + 25,20, BLACK);

}