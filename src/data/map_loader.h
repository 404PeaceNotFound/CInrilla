#ifndef MAP_LOADER_H
#define MAP_LOADER_H

#include "../data/map_data.h"

// Funções para carregar e descarregar o mapa
GameMap MapLoader_Load(const char* jsonPath);
void MapLoader_Unload(GameMap* map);

#endif