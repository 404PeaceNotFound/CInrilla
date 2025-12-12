#ifndef MAP_DATA_H
#define MAP_DATA_H

#include <raylib.h>
#include <stdbool.h>

#define MAX_LAYERS 10 // Limite de camadas (aumente se precisar)

typedef struct {
    char name[32];
    int width;
    int height;
    int* data;
    bool isVisible;
} MapLayer;

typedef struct {
    int width;
    int height;
    int tileWidth;
    int tileHeight;
    
    Texture2D texture;
    int firstGid;
    int columns;
    
    // --- LISTA DE CAMADAS ---
    MapLayer layers[MAX_LAYERS]; // Array para guardar todas as layers
    int layersCount;             // Quantas layers foram carregadas
    
    int collisionLayerIndex;     // Índice da layer que serve de chão (-1 se não achar)
    
    bool loaded;
} GameMap;

#endif