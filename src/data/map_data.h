#ifndef MAP_DATA_H
#define MAP_DATA_H

#include <raylib.h>

// Itens do ambiente (Plataformas, paredes)
typedef struct {
    Rectangle rect;
    int blocking; // 1 = bloqueia passagem, 0 = fundo
    Color color;
} EnvItem;

#endif