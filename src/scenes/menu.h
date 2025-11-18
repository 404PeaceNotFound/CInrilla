#ifndef MENU_H
#define MENU_H

#include <raylib.h>
#include "scene_manager.h" // Necessário para a definição de EstadoJogo

// Funções de ciclo de vida da cena
void InicializarMenu(void);
EstadoJogo AtualizarMenu(void);
void DesenharMenu(void);
void EncerrarMenu(void);

#endif