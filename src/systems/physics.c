#include "systems.h"
#include "../config/config.h"
#include <raymath.h>

void Physics_UpdatePlayer(Player *player, EnvItem *envItems, int envLength, float dt){
    if (player->state == PlayerDead) {
        player->speed = 0.0f; // Garante que não haja velocidade vertical residual.
        return; // Sai da função imediatamente, protegendo player->position.y.
    }

    bool hitObstacle = false;
    Vector2 *p = &(player->position);

    // Colisão vertical e chão
    for (int i = 0; i < envLength; i++) {
        EnvItem *ei = envItems + i;
        if (ei->blocking &&
            ei->rect.x <= p->x &&
            ei->rect.x + ei->rect.width >= p->x &&
            ei->rect.y >= p->y &&
            ei->rect.y <= p->y + player->speed * dt) 
        {
            hitObstacle = true;
            player->speed = 0.0f;
            p->y = ei->rect.y;
            break;
        }
    }

    if (!hitObstacle) {
        player->position.y += player->speed * dt;
        player->speed += GRAVIDADE * dt;
        player->canJump = false;
    } else {
        player->canJump = true;
    }
}
// fiisca dos inimigos
void Physics_UpdateEnemy(Enemy *enemy, EnvItem *envItem, int envLength, float dt){

    if (!enemy->active) return;

    bool hitGround = false;
    // calcula centro da hitbox dos inimigos e apenas cai quando passar do meio +1 
    float enemyCenterX = enemy->position.x + (enemy->width / 2.0f);
    //previsão proxima posição Y
    float nextY = enemy->position.y + enemy->verticalSpeed *dt;

    for(int i =0; i < envLength; i++){
        EnvItem *ei = envItem +i;

        if(!ei->blocking) continue;
        // verifica as colisões verticais
        if(ei->rect.x <= enemyCenterX && ei->rect.x + ei->rect.width >= enemyCenterX){
            //caso da colição ser no chão (caindo)
            if(enemy -> verticalSpeed >= 0 && enemy->position.y <= ei->rect.y && nextY >= ei->rect.y){
                enemy->verticalSpeed = 0.0f;
                enemy->position.y = ei->rect.y;
                hitGround = true;
                //caso de colição com o teto(pulando)
            }else if(enemy->verticalSpeed < 0 && enemy->position.y - enemy->height >= ei->rect.y + ei->rect.height && nextY - enemy->height <= ei->rect.y + ei->rect.height){
                enemy->verticalSpeed = 0.0f;
                enemy->position.y = ei->rect.y + ei->rect.height + enemy->height;

            }
        
        }
    }

    if(!hitGround){
        enemy->position.y += enemy->verticalSpeed * dt;
        enemy->verticalSpeed += GRAVIDADE * dt;
    }

    float moveX = enemy->direction * enemy->speed *dt;
    Rectangle futureHitbox = {enemy->position.x + moveX, enemy->position.y - enemy->height + 5, enemy->width, enemy->height -10};
    for(int i = 0; i < envLength; i++){
        EnvItem *ei = envItem + i;
        if(!ei->blocking) continue;

        if(CheckCollisionRecs(futureHitbox, ei->rect)){
            //se bater na parede inverte 
            enemy->direction *= -1;
            return;
        }
    }
}
    