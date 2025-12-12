#ifndef ENTITY_TYPES_H
#define ENTITY_TYPES_H

#include <raylib.h>
#include <stdbool.h>

// -----------------------------------------------------------------------------
// ENUMS (Tipos e Estados)
// -----------------------------------------------------------------------------

typedef enum {
    PlayerIdle,
    PlayerRun,
    PlayerJump,
    PlayerAtk
} PlayerState;

typedef enum {
    ENEMY_TYPE_BOAR,
    ENEMY_TYPE_SMALL_BEE,
    ENEMY_TYPE_SNAIL
} EnemyType;

typedef enum {
    ENEMY_STATE_IDLE,
    ENEMY_STATE_WALK,
    ENEMY_STATE_RUN,
    ENEMY_STATE_ATTACK
} EnemyState;

// -----------------------------------------------------------------------------
// STRUCTS AUXILIARES (Animação e Audio)
// -----------------------------------------------------------------------------

typedef struct {
    Sound Run;
    Sound Jump;
    Sound Atk;
} PlayerAudio;

// Struct completa da sua lógica de animação
typedef struct {
    Texture2D spriteSheet;
    int framesX;
    int framesY;
    int indiceFrameX;
    int indiceFrameY;
    float tempoPorFrame;
    float temporizador;
    int larguraFrame;
    int alturaFrame;
    bool inverteAnimacao;
    bool loop;
    bool ismenu;
    bool final; // Flag para indicar que animação sem loop acabou
} AnimacaoSpritesheet;

// -----------------------------------------------------------------------------
// STRUCT PLAYER
// -----------------------------------------------------------------------------

typedef struct {
    // Física
    Vector2 position;
    float speed;        // Velocidade Vertical (Y)
    bool canJump;
    
    // Lógica de Jogo
    int health;
    int damage;
    int PlayerDirection; // 1 (Direita) ou -1 (Esquerda)
    
    // Estado e Combate
    PlayerState state;
    bool isatk;
    bool hasHit; // Impede que o mesmo ataque dê dano múltiplos frames seguidos
    
    // Componentes Visuais/Sonoros
    AnimacaoSpritesheet anim[4]; // Array mapeado pelo enum PlayerState
    PlayerAudio soundPlayer;
} Player;

// -----------------------------------------------------------------------------
// STRUCT ENEMY
// -----------------------------------------------------------------------------

typedef struct {
    // Física e Movimento
    Vector2 position;
    float speed;
    float verticalSpeed;
    int direction;       // 1 ou -1
    
    // Limites de Patrulha (Antigos leftLimit/rightLimit)
    float minX; 
    float maxX; 

    // Dimensões
    float width;
    float height;
    
    // Estado
    bool active;
    int health;
    EnemyType type;
    EnemyState state;

    // Renderização e Animação
    bool useTexture;
    Texture2D texture;
    
    // Controle de Frames (Antigos frame/frameTimer)
    int frame;           
    float frameTime;     
    float timer;         // Substitui "frameTimer"
    
    // Configuração do Spritesheet
    int frameWidth;      
    int frameHeight;     
    
    // Linhas do Spritesheet
    int rowIdle;
    int rowWalk;
    int rowRun;
    int rowAttack;
    
    // Máximos de Frames
    int maxFramesIdle;
    int maxFramesWalk;
    int maxFramesRun;
    int maxFramesAttack;
} Enemy;

// Protótipo da Factory (Implementada em entities.c)
Enemy Enemy_Create(Vector2 pos, float minX, float maxX, float speed);

#endif