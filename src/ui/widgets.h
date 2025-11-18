#ifndef WIDGETS_H
#define WIDGETS_H

#include "../scenes/scene_manager.h" // Para EstadoJogo
#include <raylib.h>

typedef struct {
    Rectangle retangulo;
    const char* texto;
    EstadoJogo acao;
    bool estaSelecionado;
} Botao;

void DesenharBotao(Botao* botao);
void DesenharRodape(const char* texto);

#endif