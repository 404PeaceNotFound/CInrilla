#ifndef MAP_DATA_H
#define MAP_DATA_H

#include <raylib.h>
#include <stdbool.h>

// Estrutura de uma Camada (Layer)
typedef struct {
    char name[32];
    int width;          // Em tiles
    int height;         // Em tiles
    int* data;          // Array linear de IDs (GIDs)
    bool isVisible;
} MapLayer;

// Estrutura principal do Mapa
typedef struct {
    int width;          // Largura total em tiles
    int height;         // Altura total em tiles
    int tileWidth;      // Ex: 16px
    int tileHeight;     // Ex: 16px
    
    // Tileset Principal
    Texture2D texture;  // A imagem (Tiles.png)
    int firstGid;       // ID inicial (geralmente 1)
    int columns;        // Quantas colunas tem a imagem do tileset (para cálculo de UV)
    
    // Camadas Específicas
    MapLayer layerGround;    // Camada de colisão
    MapLayer layerDecor;     // Camada visual (plantas, pedras)
    
    // Flag de controle
    bool loaded;
} GameMap;

#endif