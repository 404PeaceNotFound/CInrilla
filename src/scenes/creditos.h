#ifndef CREDITOS_H
#define CREDITOS_H

#include <raylib.h>
#include "scene_manager.h" // Necessário para a definição de EstadoJogo

void InicializarCreditos(void);
EstadoJogo AtualizarCreditos(void);
void DesenharCreditos(void);
void EncerrarCreditos(void);

#endif