#ifndef SPRITESHEET_ANIMACAO_H
#define SPRITESHEET_ANIMACAO_H

#include <raylib.h>
#include <stdbool.h>

// Estrutura que contém o estado e as regras de uma animação de spritesheet
typedef struct {
    Texture2D spriteSheet;
    int framesX;        // Contagem de frames na horizontal
    int framesY;        // Contagem de frames na vertical
    int indiceFrameX;   // Frame atual X
    int indiceFrameY;   // Frame atual Y
    float tempoPorFrame;
    float temporizador;
    bool inverteAnimacao;
    
    // Dimensões originais da tela ou área de destino (para DrawTexturePro)
    int larguraDestino; 
    int alturaDestino;  
} AnimacaoSpritesheet;

// Funções de ciclo de vida e manipulação
AnimacaoSpritesheet CriarAnimacaoSpritesheet(
    const char *caminho, 
    int framesX, 
    int framesY, 
    float fps, 
    int larguraDestino,
    int alturaDestino
);

void AtualizarAnimacaoSpritesheet(AnimacaoSpritesheet *anim, float dt);
void DesenharAnimacaoSpritesheet(AnimacaoSpritesheet anim, Vector2 centroDestino);
void EncerrarAnimacaoSpritesheet(AnimacaoSpritesheet *anim);

#endif