// enemies.h
#ifndef ENEMIES_H
#define ENEMIES_H

#include <raylib.h>   

typedef enum {
    ENEMY_STATE_IDLE,
    ENEMY_STATE_WALK,
    ENEMY_STATE_RUN,
    ENEMY_STATE_ATTACK
} EnemyState;

typedef struct Enemy {
    Vector2 position;

    float leftLimit;
    float rightLimit;
    float speed;
    int direction;      // -1 esquerda, 1 direita

    float width;
    float height;

    // controle de animação
    int frame;
    float frameTime;
    float frameTimer;

    int frameWidth;
    int frameHeight;

    int maxFramesIdle;
    int maxFramesWalk;
    int maxFramesRun;
    int maxFramesAttack;

    int rowIdle;
    int rowWalk;
    int rowRun;
    int rowAttack;

    EnemyState state;

    int active;         // 1 = ativo, 0 = desligado
} Enemy;

// inicializa um inimigo já alocado
void InitEnemy(Enemy *e, Vector2 pos, float leftLimit, float rightLimit, float speed);

// cria e retorna um Enemy pronto
Enemy Enemy_Create(Vector2 pos, float leftLimit, float rightLimit, float walkSpeed);

// atualiza lógica
void UpdateEnemy(Enemy *e, float dt);

// desenha (só um retângulo)
void DrawEnemy(Enemy *e);

#endif