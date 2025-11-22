#include "systems.h"
#include <raylib.h>

// --- Map & Player Rendering ---
void Render_Map(EnvItem *envItems, int envLength) {
    for (int i = 0; i < envLength; i++) {
        DrawRectangleRec(envItems[i].rect, envItems[i].color);
    }
}

void Render_Player(Player *player) {
    Rectangle playerRect = { player->position.x - 20, player->position.y - 40, 40.0f, 40.0f };
    DrawRectangleRec(playerRect, RED);
    DrawCircleV(player->position, 5.0f, GOLD);
}

void Render_UpdateCamera(Camera2D *camera, Player *player, int width, int height) {
    camera->offset = (Vector2){ width/2.0f, height/2.0f };
    camera->target = player->position;
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