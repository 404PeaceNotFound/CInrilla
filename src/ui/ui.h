#ifndef UI_H
#define UI_H

#include <raylib.h>
#include <stdbool.h>

typedef struct {
    Rectangle retangulo;
    const char* texto;
    bool estaSelecionado;
} BotaoUI;

void UI_DesenharBotao(BotaoUI* botao);
void UI_DesenharTextoCentralizado(const char* texto, int y, int tamanho, Color cor);

#endif