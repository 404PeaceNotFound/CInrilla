#ifndef ENTITY_TYPES_H
#define ENTITY_TYPES_H

#include <raylib.h>
#include <stdbool.h>

// --- ENUMS ---
typedef enum {
    PlayerIdle,
    PlayerRun,
    PlayerJump,
    PlayerAtk,
    PlayerDead,
    PlayerCount
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

// --- STRUCTS AUXILIARES ---
typedef struct {
    Sound Run;
    Sound Jump;
    Sound Atk;
} PlayerAudio;

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
    bool final;
} AnimacaoSpritesheet;

// --- PLAYER ---
typedef struct {
    Vector2 position;
    float speed;
    bool canJump;
    
    int health;
    int damage;
    int PlayerDirection; // 1 ou -1
    
    PlayerState state;
    bool isatk;
    bool hasHit; 
    float hurtTimer; // Invencibilidade
    float renderoffsetY;
    
    AnimacaoSpritesheet anim[PlayerCount]; 
    PlayerAudio soundPlayer;
} Player;

// --- ENEMY ---
typedef struct {
    Vector2 position;
    float speed;
    float verticalSpeed;
    int direction;       
    
    float minX; 
    float maxX; 

    float width;
    float height;
    
    bool active;
    int health;
    int damage;
    float renderoffsetY;
    EnemyType type;
    EnemyState state;

    bool useTexture;
    Texture2D texture;
    
    int frame;           
    float frameTime;  
    float frameTimer;   
    float timer;         
    
    int frameWidth;      
    int frameHeight;     
    
    int rowIdle;
    int rowWalk;
    int rowRun;
    int rowAttack;
    
    int maxFramesIdle;
    int maxFramesWalk;
    int maxFramesRun;
    int maxFramesAttack;
} Enemy;

#endif