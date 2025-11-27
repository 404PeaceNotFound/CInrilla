#include <raylib.h>
#include <raymath.h> // Necessário para fminf e fmaxf (ou o Clamp da Raymath)
#include "systems.h"
#include "../config/config.h"

// Função interna auxiliar
static void DrawLayer(MapLayer* layer, GameMap* map) {
    if (!layer->data || !map->texture.id) return;

    for (int y = 0; y < layer->height; y++) {
        for (int x = 0; x < layer->width; x++) {
            // Índice no array linear
            int index = y * layer->width + x;
            int gid = layer->data[index];

            // Se gid for 0, é vazio
            if (gid == 0) continue;

            // Ajuste do ID para índice do tileset (Tiled começa em 1)
            int tilesetIndex = gid - map->firstGid;
            if (tilesetIndex < 0) continue;

            // Matemática para achar o retângulo de recorte (Source Rect) na textura
            int textureX = (tilesetIndex % map->columns) * map->tileWidth;
            int textureY = (tilesetIndex / map->columns) * map->tileHeight;

            Rectangle source = { 
                (float)textureX, 
                (float)textureY, 
                (float)map->tileWidth, 
                (float)map->tileHeight 
            };

            // Onde desenhar no mundo
            Vector2 position = { 
                (float)x * map->tileWidth, 
                (float)y * map->tileHeight 
            };

            DrawTextureRec(map->texture, source, position, WHITE);
        }
    }
}

// --- Map & Player Rendering ---
void Render_Map(GameMap* map) {
    if (!map->loaded) return;

    // Desenha camadas em ordem
    DrawLayer(&map->layerGround, map);
    DrawLayer(&map->layerDecor, map);
}


void Render_Player(Player *player) {
    Rectangle playerRect = { player->position.x - 20, player->position.y - 40, 40.0f, 40.0f };
    DrawRectangleRec(playerRect, RED);
    DrawCircleV(player->position, 5.0f, GOLD);
}

void Render_UpdateCamera(Camera2D *camera, Player *player, GameMap* map, int width, int height) {
    // 1. Configuração base (segue o jogador)
    // O offset permanece fixo (centralizado)
    camera->offset = (Vector2){ width/2.0f, height/2.0f };
    camera->target = player->position;

    if (!map->loaded) return;
    // 2. Determinar limites do mundo (em coordenadas de mundo)
    float minX_world = 0.0f;
    float minY_world = 0.0f;
    float maxX_world = (float)map->width * map->tileWidth;
    float maxY_world = (float)map->height * map->tileHeight;
    // 3. Determinar os limites MÍNIMO e MÁXIMO que o camera->target pode assumir.
    float targetMinX = minX_world + width / 2.0f / camera->zoom; // Target Mínimo: O ponto mais à esquerda/topo que o Target pode ir antes que a borda do mapa
    float targetMinY = minY_world + height / 2.0f / camera->zoom;
    float targetMaxX = maxX_world - width / 2.0f / camera->zoom; // Target Máximo: O ponto mais à direita/baixo que o Target pode ir antes que a borda do mapa
    float targetMaxY = maxY_world - height / 2.0f / camera->zoom;
    // 4. Clamping (Restrição do Alvo)
    // Garante que o target mínimo não ultrapasse o máximo (caso o mapa seja menor que a tela na dimensão)
    if (targetMinX > targetMaxX) {
        targetMinX = targetMaxX = (minX_world + maxX_world) / 2.0f; // Se o mapa for menor que a tela, centraliza o target no mapa
    }
    if (targetMinY > targetMaxY) {
        targetMinY = targetMaxY = (minY_world + maxY_world) / 2.0f; // Se o mapa for menor que a tela, centraliza o target no mapa
    }
    // Aplica o Clamping final ao camera->target
    camera->target.x = fmaxf(camera->target.x, targetMinX);
    camera->target.x = fminf(camera->target.x, targetMaxX);
    camera->target.y = fmaxf(camera->target.y, targetMinY);
    camera->target.y = fminf(camera->target.y, targetMaxY);
}

// --- Animation System Logic (Ex-spritesheet_animacao) ---

AnimacaoSpritesheet Render_CreateAnim(const char *path, int fX, int fY, float fps, int w, int h) {
    AnimacaoSpritesheet anim = {0};
    anim.spriteSheet = LoadTexture(path);
    anim.framesX = fX;
    anim.framesY = fY;
    anim.tempoPorFrame = 1.0f / fps;
    anim.temporizador = anim.tempoPorFrame;
    anim.larguraDestino = w;
    anim.alturaDestino = h;
    anim.inverteAnimacao = false;
    return anim;
}

void Render_UpdateAnim(AnimacaoSpritesheet *anim, float dt) {
    anim->temporizador -= dt;
    if (anim->temporizador < 0) {
        anim->temporizador = anim->tempoPorFrame;
        
        if (!anim->inverteAnimacao) {
            anim->indiceFrameX++;
            if (anim->indiceFrameX >= anim->framesX) {
                anim->indiceFrameX = 0;
                anim->indiceFrameY++;
            }
        } else {
            anim->indiceFrameX--;
            if (anim->indiceFrameX < 0) {
                anim->indiceFrameX = anim->framesX - 1;
                anim->indiceFrameY--;
            }
        }
        
        // Check boundaries for loop/invert
        if (anim->indiceFrameX >= anim->framesX - 1 && anim->indiceFrameY >= anim->framesY - 1)
            anim->inverteAnimacao = true;
        else if (anim->indiceFrameX <= 0 && anim->indiceFrameY <= 0)
            anim->inverteAnimacao = false;
    }
}

void Render_DrawAnim(AnimacaoSpritesheet anim, Vector2 centroDestino) {
    Rectangle origem = {
        anim.indiceFrameX * anim.larguraDestino,
        anim.indiceFrameY * anim.alturaDestino,
        anim.larguraDestino,
        anim.alturaDestino
    };
    Rectangle destino = {
        centroDestino.x, centroDestino.y,
        origem.width, origem.height
    };
    DrawTexturePro(anim.spriteSheet, origem, destino, 
        (Vector2){destino.width/2.0f, destino.height/2.0f}, 0, WHITE);
}

void Render_UnloadAnim(AnimacaoSpritesheet *anim) {
    if (anim->spriteSheet.id != 0) {
        UnloadTexture(anim->spriteSheet);
        anim->spriteSheet.id = 0;
    }
}