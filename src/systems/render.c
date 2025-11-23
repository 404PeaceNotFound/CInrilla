#include "systems.h"
#include <raylib.h>

// --- Map & Player Rendering ---
void Render_Map(EnvItem *envItems, int envLength) {
    for (int i = 0; i < envLength; i++) {
        DrawRectangleRec(envItems[i].rect, envItems[i].color);
    }
}

void initPlayer(Player *player){
    //Começa Parado
    player->state = PlayerIdle;
    player->isatk = false;
    player->PlayerDirection = 1;

    player->anim[PlayerIdle] = Render_CreateAnim("assets/sprites/character/Idle/Idle-Sheet.png", //IDLE
        4,   // framesX 
        1,   // framesY
        6,   // fps (0 para estático)
        64,  // largura do frame
        64   // altura do frame
        );
    player->anim[PlayerRun] = Render_CreateAnim("assets/sprites/character/Run/Run-Sheet.png", //CORRER
        8,   // framesX 
        1,   // framesY
        6,   // fps (0 para estático)
        80,  // largura do frame
        80   // altura do frame
        );
    player->anim[PlayerJump] = Render_CreateAnim("assets/sprites/character/Jumlp-All/Jump-All-Sheet.png", //PULAR
        15,   // framesX 
        1,   // framesY
        6,   // fps (0 para estático)
        64,  // largura do frame
        64   // altura do frame
        );
    player->anim[PlayerAtk] = Render_CreateAnim("assets/sprites/character/Attack-01/Attack-01-Sheet.png", //Atacar
        8,   // framesX 
        1,   // framesY
        6,   // fps (0 para estático)
        96,  // largura do frame
        80   // altura do frame
        );

    player->anim[player->state].indiceFrameX = 0;
    player->anim[player->state].indiceFrameY = 0; 
    }

void Render_Player(Player *player) {

    if(player->anim[player->state].spriteSheet.id > 0){

      //  if(player->PlayerDirection == -1 && (player->state != PlayerIdle)){
       //     player->anim[player->state].inverteAnimacao = true;
     //   }
      //  else{
       //     player->anim[player->state].inverteAnimacao = false;

       // }

        Vector2 posicaoVisual = player->position;
        posicaoVisual.y -= 32.0f; //offset
        Render_DrawAnim(player->anim[player->state], posicaoVisual); //Carregar protagonista
    }

    //DrawCircleV(player->position, 5.0f, GOLD); Debug
}

void Render_UpdateCamera(Camera2D *camera, Player *player, int width, int height) {
    camera->offset = (Vector2){ width/2.0f, height/2.0f };
    camera->target = player->position;
}

// --- Animation System Logic (Ex-spritesheet_animacao) ---

AnimacaoSpritesheet Render_CreateAnim(const char *path, int fX, int fY, float fps, int w, int h) { //Carrega Sprite
    AnimacaoSpritesheet anim = {0};
    anim.spriteSheet = LoadTexture(path);
    anim.framesX = fX;
    anim.framesY = fY;
    anim.tempoPorFrame = 1.0f / fps;
    anim.temporizador = anim.tempoPorFrame;
    anim.larguraFrame = w;
    anim.alturaFrame = h;
    anim.inverteAnimacao = false;
    return anim;
}

void Render_UpdateAnim(AnimacaoSpritesheet *anim, float dt) { //motor dos sprites
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
        anim.indiceFrameX * anim.larguraFrame,
        anim.indiceFrameY * anim.alturaFrame,
        anim.larguraFrame,
        anim.alturaFrame
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