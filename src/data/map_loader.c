#include "map_loader.h"
#include "../core/cJSON.h" // Certifique-se que este arquivo existe
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

// Extrai apenas o nome do arquivo de um caminho completo (ex: ../img/Tiles.png -> Tiles.png)
static const char* GetFileNameFromPath(const char* path) {
    const char* filename = strrchr(path, '/'); // Linux/Mac
    if (!filename) filename = strrchr(path, '\\'); // Windows
    
    if (filename) return filename + 1;
    return path;
}

GameMap MapLoader_Load(const char* jsonPath) {
    GameMap map = {0};
    map.loaded = false;

    char* jsonContent = LoadFileText_Custom(jsonPath);
    if (!jsonContent) {
        printf("ERRO [MapLoader]: Nao foi possivel abrir %s\n", jsonPath);
        return map;
    }

    cJSON* root = cJSON_Parse(jsonContent);
    free(jsonContent); // Libera texto bruto, agora está na struct cJSON

    if (!root) {
        printf("ERRO [MapLoader]: Falha no parsing JSON.\n");
        return map;
    }

    // 1. Ler Dimensões Globais
    map.width = cJSON_GetObjectItem(root, "width")->valueint;
    map.height = cJSON_GetObjectItem(root, "height")->valueint;
    map.tileWidth = cJSON_GetObjectItem(root, "tilewidth")->valueint;
    map.tileHeight = cJSON_GetObjectItem(root, "tileheight")->valueint;

    // 2. Ler Tileset (Assume-se o primeiro tileset para a textura principal)
    cJSON* tilesets = cJSON_GetObjectItem(root, "tilesets");
    if (cJSON_IsArray(tilesets)) {
        cJSON* ts = cJSON_GetArrayItem(tilesets, 0); // Pega o primeiro
        
        map.firstGid = cJSON_GetObjectItem(ts, "firstgid")->valueint;
        map.columns = cJSON_GetObjectItem(ts, "columns")->valueint;
        
        // Tratamento do caminho da imagem
        cJSON* imgPath = cJSON_GetObjectItem(ts, "image");
        if (imgPath) {
            const char* cleanName = GetFileNameFromPath(imgPath->valuestring);
            char fullPath[128];
            // Força o carregamento de assets/sprites/
            snprintf(fullPath, sizeof(fullPath), "assets/sprites/%s", cleanName);
            
            map.texture = LoadTexture(fullPath);
            if (map.texture.id == 0) {
                 printf("ERRO [MapLoader]: Textura nao encontrada em %s\n", fullPath);
            }
        }
    }

    // 3. Ler Camadas (Layers)
    cJSON* layers = cJSON_GetObjectItem(root, "layers");
    cJSON* layer = NULL;

    cJSON_ArrayForEach(layer, layers) {
        const char* name = cJSON_GetObjectItem(layer, "name")->valuestring;
        const char* type = cJSON_GetObjectItem(layer, "type")->valuestring;
        
        // Filtramos apenas tilelayers
        if (strcmp(type, "tilelayer") != 0) continue;

        MapLayer* targetLayer = NULL;

        // Mapeia nomes do JSON para nossas structs
        if (strcmp(name, "Ground") == 0) targetLayer = &map.layerGround;
        else if (strcmp(name, "Plants and Rocks") == 0) targetLayer = &map.layerDecor;

        if (targetLayer) {
            strncpy(targetLayer->name, name, 31);
            targetLayer->width = cJSON_GetObjectItem(layer, "width")->valueint;
            targetLayer->height = cJSON_GetObjectItem(layer, "height")->valueint;
            targetLayer->isVisible = cJSON_GetObjectItem(layer, "visible")->valueint;
            
            cJSON* dataArray = cJSON_GetObjectItem(layer, "data");
            int size = cJSON_GetArraySize(dataArray);
            
            targetLayer->data = (int*)malloc(size * sizeof(int));
            
            int i = 0;
            cJSON* val = NULL;
            cJSON_ArrayForEach(val, dataArray) {
                targetLayer->data[i++] = val->valueint;
            }
        }
    }

    map.loaded = true;
    cJSON_Delete(root);
    printf("SUCESSO [MapLoader]: Mapa carregado (%dx%d)\n", map.width, map.height);
    
    return map;
}

void MapLoader_Unload(GameMap* map) {
    if (!map->loaded) return;

    UnloadTexture(map->texture);
    if (map->layerGround.data) free(map->layerGround.data);
    if (map->layerDecor.data) free(map->layerDecor.data);

    map->loaded = false;
}