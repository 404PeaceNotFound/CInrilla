#include "systems.h"
#include "../config/config.h"
#include <raylib.h>

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

void Render_UpdateCamera(Camera2D *camera, Player *player, EnvItem *envItems, int envItemsLength, int width, int height) {
    camera->target = player->position;
    camera->offset = (Vector2){ width/2.0f, height/2.0f };
    float minX = 1000, minY = 1000, maxX = -1000, maxY = -1000;

    for (int i = 0; i < envItemsLength; i++)
    {
        EnvItem *ei = envItems + i;
        minX = fminf(ei->rect.x, minX);
        maxX = fmaxf(ei->rect.x + ei->rect.width, maxX);
        minY = fminf(ei->rect.y, minY);
        maxY = fmaxf(ei->rect.y + ei->rect.height, maxY);
    }

    Vector2 max = GetWorldToScreen2D((Vector2){ maxX, maxY }, *camera);
    Vector2 min = GetWorldToScreen2D((Vector2){ minX, minY }, *camera);

    if (max.x < width) camera->offset.x = width - (max.x - width/2);
    if (max.y < height) camera->offset.y = height - (max.y - height/2);
    if (min.x > 0) camera->offset.x = width/2 - min.x;
    if (min.y > 0) camera->offset.y = height/2 - min.y;
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