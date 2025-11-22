#ifndef ENTITY_TYPES_H
#define ENTITY_TYPES_H

#include <raylib.h>
#include <stdbool.h>

// Estrutura para Animação (Migrada de spritesheet_animacao)
typedef struct {
    Texture2D spriteSheet;
    int framesX;
    int framesY;
    int indiceFrameX;
    int indiceFrameY;
    float tempoPorFrame;
    float temporizador;
    bool inverteAnimacao;
    int larguraDestino;
    int alturaDestino;
} AnimacaoSpritesheet;

// Player
typedef struct {
    Vector2 position;
    float speed; // Velocidade vertical (Y)
    bool canJump;
    int health;
} Player;

#endif