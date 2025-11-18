#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include <stdbool.h>

typedef enum {
    TELA_MENU,
    TELA_JOGO,
    TELA_CREDITOS,
    TELA_SAIR
} EstadoJogo;

void InicializarGerenciadorCenas(void);
void AtualizarGerenciadorCenas(void);
void DesenharGerenciadorCenas(void);
void EncerrarGerenciadorCenas(void);
bool DeveFecharJogo(void);
void MudarEstado(EstadoJogo novoEstado);

#endif