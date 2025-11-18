#include "spritesheet_animacao.h"
#include <raymath.h>

AnimacaoSpritesheet CriarAnimacaoSpritesheet(
    const char *caminho, 
    int framesX, 
    int framesY, 
    float fps, 
    int larguraDestino,
    int alturaDestino
) {
    AnimacaoSpritesheet anim = {0};
    anim.spriteSheet = LoadTexture(caminho);
    anim.framesX = framesX;
    anim.framesY = framesY;
    anim.tempoPorFrame = 1.0f / fps;
    anim.temporizador = anim.tempoPorFrame;
    anim.larguraDestino = larguraDestino;
    anim.alturaDestino = alturaDestino;
    anim.inverteAnimacao = false;
    
    return anim;
}

void AtualizarAnimacaoSpritesheet(AnimacaoSpritesheet *anim, float dt) {
    anim->temporizador -= dt;

    if (anim->temporizador < 0) {
        anim->temporizador = anim->tempoPorFrame;

        // Lógica de avanço/retrocesso do frame
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
        
        // Lógica de inversão da animação
        if (anim->indiceFrameX == anim->framesX - 1 && anim->indiceFrameY == anim->framesY - 1) {
            anim->inverteAnimacao = true;
        } else if (anim->indiceFrameX <= 0 && anim->indiceFrameY <= 0) {
            anim->inverteAnimacao = false;
        }
    }
}

void DesenharAnimacaoSpritesheet(AnimacaoSpritesheet anim, Vector2 centroDestino) {
    // Cálculo da área de origem na spritesheet
    Rectangle origem = (Rectangle){
        anim.indiceFrameX * anim.larguraDestino, 
        anim.indiceFrameY * anim.alturaDestino, 
        anim.larguraDestino, 
        anim.alturaDestino
    };
    
    // Cálculo da área de destino (centralizada)
    Rectangle destino = (Rectangle){
        centroDestino.x, 
        centroDestino.y, 
        origem.width, 
        origem.height
    };
    
    DrawTexturePro(
        anim.spriteSheet, 
        origem, 
        destino, 
        (Vector2){destino.width/2.0f, destino.height/2.0f}, // Origem de rotação
        0, 
        WHITE
    );
}

void EncerrarAnimacaoSpritesheet(AnimacaoSpritesheet *anim) {
    UnloadTexture(anim->spriteSheet);
    anim->spriteSheet.id = 0; // Marca como descarregada
}