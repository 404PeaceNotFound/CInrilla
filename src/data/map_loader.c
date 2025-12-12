#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "map_loader.h"
#include "../core/cJSON.h"

// Função auxiliar para ler arquivo texto inteiro para memória
static char* LoadFileText_Custom(const char* fileName) {
    FILE* file = fopen(fileName, "rt");
    if (!file) return NULL;

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* buffer = (char*)malloc(length + 1);
    if (!buffer) { fclose(file); return NULL; }

    fread(buffer, 1, length, file);
    buffer[length] = '\0';
    
    fclose(file);
    return buffer;
}

// Extrai apenas o nome do arquivo de um caminho completo
static const char* GetFileNameFromPath(const char* path) {
    const char* filename = strrchr(path, '/'); // Linux/Mac
    if (!filename) filename = strrchr(path, '\\'); // Windows
    
    if (filename) return filename + 1;
    return path;
}

GameMap MapLoader_Load(const char* jsonPath) {
    printf("--- DEBUG: Carregando mapa (Sistema de Array): %s ---\n", jsonPath);
    GameMap map = {0};
    map.loaded = false;
    map.collisionLayerIndex = -1; // Começa sem colisão definida

    char* jsonContent = LoadFileText_Custom(jsonPath);
    if (!jsonContent) return map;

    cJSON* root = cJSON_Parse(jsonContent);
    free(jsonContent);
    if (!root) return map;

    // 1. Dimensões
    map.width = cJSON_GetObjectItem(root, "width")->valueint;
    map.height = cJSON_GetObjectItem(root, "height")->valueint;
    map.tileWidth = cJSON_GetObjectItem(root, "tilewidth")->valueint;
    map.tileHeight = cJSON_GetObjectItem(root, "tileheight")->valueint;

    // 2. Tileset
    cJSON* tilesets = cJSON_GetObjectItem(root, "tilesets");
    if (cJSON_IsArray(tilesets)) {
        cJSON* ts = cJSON_GetArrayItem(tilesets, 0);
        if (ts) {
            map.firstGid = cJSON_GetObjectItem(ts, "firstgid")->valueint;
            map.columns = cJSON_GetObjectItem(ts, "columns")->valueint;
            
            cJSON* imgPath = cJSON_GetObjectItem(ts, "image");
            if (imgPath) {
                const char* cleanName = GetFileNameFromPath(imgPath->valuestring);
                char fullPath[256];
                snprintf(fullPath, sizeof(fullPath), "assets/images/tiles/%s", cleanName);
                map.texture = LoadTexture(fullPath);
            }
        }
    }

    // 3. Camadas (Loop Genérico)
    cJSON* layers = cJSON_GetObjectItem(root, "layers");
    cJSON* layer = NULL;

    cJSON_ArrayForEach(layer, layers) {
        // Proteção contra limite de layers
        if (map.layersCount >= MAX_LAYERS) {
            printf("AVISO: Maximo de layers atingido (%d)!\n", MAX_LAYERS);
            break;
        }

        cJSON* typeItem = cJSON_GetObjectItem(layer, "type");
        const char* type = typeItem ? typeItem->valuestring : "";
        
        // Só carrega se for tilelayer
        if (strcmp(type, "tilelayer") != 0) continue;

        // Pega a próxima vaga livre no array
        MapLayer* targetLayer = &map.layers[map.layersCount];
        cJSON* nameItem = cJSON_GetObjectItem(layer, "name");
        const char* name = nameItem ? nameItem->valuestring : "Unamed";

        // Copia dados básicos
        strncpy(targetLayer->name, name, 31);
        targetLayer->width = cJSON_GetObjectItem(layer, "width")->valueint;
        targetLayer->height = cJSON_GetObjectItem(layer, "height")->valueint;
        targetLayer->isVisible = cJSON_GetObjectItem(layer, "visible")->valueint;

        // Carrega Tiles
        cJSON* dataArray = cJSON_GetObjectItem(layer, "data");
        if (dataArray) {
            int size = cJSON_GetArraySize(dataArray);
            targetLayer->data = (int*)malloc(size * sizeof(int));
            int i = 0;
            cJSON* val = NULL;
            cJSON_ArrayForEach(val, dataArray) {
                targetLayer->data[i++] = val->valueint;
            }
        }

        printf("Layer [%d] carregada: '%s'\n", map.layersCount, name);

        // Verifica se essa é a layer de colisão
        if (strstr(name, "Ground") != NULL || strstr(name, "Colisao") != NULL) {
            map.collisionLayerIndex = map.layersCount;
            printf("   -> Definida como COLISAO\n");
        }

        map.layersCount++; // Avança para a próxima
    }

    map.loaded = true;
    cJSON_Delete(root);
    return map;
}

void MapLoader_Unload(GameMap* map) {
    if (!map->loaded) return;
    UnloadTexture(map->texture);
    
    // Libera todas as layers do array
    for (int i = 0; i < map->layersCount; i++) {
        if (map->layers[i].data) {
            free(map->layers[i].data);
            map->layers[i].data = NULL;
        }
    }
    
    map->loaded = false;
}