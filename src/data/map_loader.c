#include "map_loader.h"
#include "../core/cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // Para tolower

// Função auxiliar para carregar texto do arquivo
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
    const char* filename = strrchr(path, '/');
    if (!filename) filename = strrchr(path, '\\');
    
    if (filename) return filename + 1;
    return path;
}

// Função para converter string para minúsculo
void str_tolower_inplace(char *str) {
    for(int i = 0; str[i]; i++){
        str[i] = tolower(str[i]);
    }
}

GameMap MapLoader_Load(const char* jsonPath) {
    printf("--- DEBUG: Carregando mapa: %s ---\n", jsonPath);
    GameMap map = {0};
    map.loaded = false;
    map.collisionLayerIndex = -1;

    // Carregar Texto do Arquivo
    char* jsonContent = LoadFileText_Custom(jsonPath);
    if (!jsonContent) {
        printf("ERRO FATAL: Nao foi possivel ler o arquivo %s\n", jsonPath);
        return map;
    }

    // Parsear JSON
    cJSON* root = cJSON_Parse(jsonContent);
    free(jsonContent);

    if (!root) {
        printf("ERRO FATAL: JSON invalido ou corrompido.\n");
        return map;
    }

    // Ler Dimensões
    map.width = cJSON_GetObjectItem(root, "width")->valueint;
    map.height = cJSON_GetObjectItem(root, "height")->valueint;
    map.tileWidth = cJSON_GetObjectItem(root, "tilewidth")->valueint;
    map.tileHeight = cJSON_GetObjectItem(root, "tileheight")->valueint;

    // Ler Tileset e Textura
    cJSON* tilesets = cJSON_GetObjectItem(root, "tilesets");
    if (cJSON_IsArray(tilesets)) {
        cJSON* ts = cJSON_GetArrayItem(tilesets, 0);
        if (ts) {
            map.firstGid = cJSON_GetObjectItem(ts, "firstgid")->valueint;
            map.columns = cJSON_GetObjectItem(ts, "columns")->valueint;
            
            cJSON* imgPath = cJSON_GetObjectItem(ts, "image");
            if (imgPath) 
            {
                const char* cleanName = GetFileNameFromPath(imgPath->valuestring);
                char fullPath[256];
                snprintf(fullPath, sizeof(fullPath), "assets/images/tiles/%s", cleanName);
                
                map.texture = LoadTexture(fullPath);
                if(map.texture.id == 0) 
                {
                    printf("AVISO: Textura nao encontrada em %s. Tentando pasta sprites...\n", fullPath);
                    snprintf(fullPath, sizeof(fullPath), "assets/sprites/%s", cleanName);
                    map.texture = LoadTexture(fullPath);
                }
            }
        }
    }

    // Ler Camadas
    cJSON* layers = cJSON_GetObjectItem(root, "layers");
    cJSON* layer = NULL;

    cJSON_ArrayForEach(layer, layers) 
    {
        if (map.layersCount >= MAX_LAYERS) break;

        cJSON* typeItem = cJSON_GetObjectItem(layer, "type");
        const char* type = typeItem ? typeItem->valuestring : "";
        
        // Ignora layers que não sejam de tiles
        if (strcmp(type, "tilelayer") != 0) continue;

        // Configura a nova layer na struct
        MapLayer* targetLayer = &map.layers[map.layersCount];
        cJSON* nameItem = cJSON_GetObjectItem(layer, "name");
        const char* rawName = nameItem ? nameItem->valuestring : "Unamed";
        
        strncpy(targetLayer->name, rawName, 31);
        targetLayer->width = cJSON_GetObjectItem(layer, "width")->valueint;
        targetLayer->height = cJSON_GetObjectItem(layer, "height")->valueint;
        targetLayer->isVisible = cJSON_GetObjectItem(layer, "visible")->valueint;

        // Aloca e copia os dados dos tiles
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

        // --- DETECÇÃO DE COLISÃO ---
        char lowerName[32];
        strncpy(lowerName, rawName, 31);
        str_tolower_inplace(lowerName);

        if (strstr(lowerName, "ground") != NULL || 
            strstr(lowerName, "colisao") != NULL || 
            strstr(lowerName, "collision") != NULL ||
            strstr(lowerName, "chao") != NULL) {
            
            map.collisionLayerIndex = map.layersCount;
            printf("   -> Layer '%s' definida como COLISAO (por nome).\n", rawName);
        }

        map.layersCount++; 
    }

    if (map.collisionLayerIndex == -1 && map.layersCount > 0) {
        printf("AVISO: Nenhuma layer 'Ground' encontrada. Usando a PRIMEIRA layer como colisao.\n");
        map.collisionLayerIndex = 0; 
    }

    map.loaded = true;
    
    // Limpeza do JSON
    cJSON_Delete(root);
    
    return map;
}

void MapLoader_Unload(GameMap* map) {
    if (!map->loaded) return;
    
    UnloadTexture(map->texture);
    
    // Libera a memória de todas as layers carregadas
    for (int i = 0; i < map->layersCount; i++) {
        if (map->layers[i].data) {
            free(map->layers[i].data);
            map->layers[i].data = NULL;
        }
    }
    
    map->loaded = false;
    map->layersCount = 0;
}