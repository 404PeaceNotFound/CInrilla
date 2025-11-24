#ifndef ENTITY_TYPES_H
#define ENTITY_TYPES_H

#include <raylib.h>
#include <stdbool.h>

// Estrutura para Animação (Migrada de spritesheet_animacao)

typedef enum{
    PlayerIdle, //0
    PlayerRun, //1
    PlayerJump, //2
    PlayerAtk, //3
    PlayerStateCount, //4
}PlayerState;

typedef struct {
    Texture2D spriteSheet;
    int framesX;
    int framesY;
    int indiceFrameX;
    int indiceFrameY;
    float tempoPorFrame;
    float temporizador;
    bool inverteAnimacao;
    int larguraFrame;
    int alturaFrame;
    bool loop;
    bool ismenu;
    bool final;
} AnimacaoSpritesheet;

typedef struct{
    Sound Run;
    Sound Atk;
    Sound Jump;
   
}Sounds;

// Player
typedef struct {
    Vector2 position;
    float speed; // Velocidade vertical (Y)
    bool canJump;
    bool isatk;
    int health;
    int PlayerDirection;
    AnimacaoSpritesheet anim[PlayerStateCount];
    PlayerState state;
    Sounds soundPlayer;
} Player;

#endif