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
    // Física e Lógica Básica
    Vector2 position;
    float speed;         // Velocidade Horizontal de movimento
    float verticalSpeed; // Velocidade Vertical (Gravidade)
    int direction;       // 1 ou -1
    
    // Dimensões de Colisão (Hitbox)
    float width;
    float height;
    
    // Estado
    bool active;
    int health;
    EnemyType type;
    EnemyState state;

    // IA Simples (Patrulha)
    float minX; // Limite esquerdo da patrulha
    float maxX; // Limite direito da patrulha

    // Renderização e Controle de Sprites
    bool useTexture;
    Texture2D texture;
    
    int frame;           // Frame atual (0 a maxFrames)
    float frameTime;     // Tempo por frame (ex: 0.1f)
    float timer;         // Acumulador de tempo delta
    
    // Configuração do Spritesheet (Preenchido pelo Render_ConfigEnemy)
    int frameWidth;      // Largura de 1 frame na imagem
    int frameHeight;     // Altura de 1 frame na imagem
    
    // Mapeamento de Linhas (Rows) no Spritesheet
    int rowIdle;
    int rowWalk;
    int rowRun;
    int rowAttack;
    
    // Quantidade de frames por animação
    int maxFramesIdle;
    int maxFramesWalk;
    int maxFramesRun;
} Enemy;

// Protótipo da Factory (Implementada em entities.c)
Enemy Enemy_Create(Vector2 pos, float minX, float maxX, float speed);

#endif