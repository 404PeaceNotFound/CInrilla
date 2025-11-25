#include "systems.h"
#include "enemy_system.h"
#include <raylib.h>
#include <math.h>

static Texture2D texBoar;
static Texture2D texBee;
static Texture2D texSnail;

// --- Map & Player Rendering ---
void Render_Map(EnvItem *envItems, int envLength) {
    for (int i = 0; i < envLength; i++){
        DrawRectangleRec(envItems[i].rect, envItems[i].color);
    }
}

void initPlayer(Player *player){
    //Começa Parado
    player->state = PlayerIdle;
    player->isatk = false;
    player->PlayerDirection = 1;
    player->damage = 5;
    player->health = 10;

    //Carregar Sons
    player->soundPlayer.Run = LoadSound("assets/sounds/Player/Run.mp3");
    player->soundPlayer.Atk = LoadSound("assets/sounds/Player/Atk.mp3");
    player->soundPlayer.Jump = LoadSound("assets/sounds/Player/Jump.mp3");

    player->anim[PlayerIdle] = Render_CreateAnim("assets/sprites/character/Idle/Idle-Sheet.png", //IDLE
        4,   // framesX 
        1,   // framesY
        6,   // fps (0 para estático)
        64,  // largura do frame
        64,   // altura do frame
        true, false);
    player->anim[PlayerRun] = Render_CreateAnim("assets/sprites/character/Run/Run-Sheet.png", //CORRER
        8,   // framesX 
        1,   // framesY
        7,   // fps (0 para estático)
        80,  // largura do frame
        64,   // altura do frame
        true, false);
    player->anim[PlayerJump] = Render_CreateAnim("assets/sprites/character/Jumlp-All/Jump-All-Sheet.png", //PULAR
        15,   // framesX 
        1,   // framesY
        6,   // fps (0 para estático)
        64,  // largura do frame
        64,   // altura do frame
        true, false);
    player->anim[PlayerAtk] = Render_CreateAnim("assets/sprites/character/Attack-01/Attack-01-Sheet.png", //Atacar
        8,   // framesX 
        1,   // framesY
        8,   // fps (0 para estático)
        96,  // largura do frame
        64,   // altura do frame
        false, false);

    player->anim[player->state].indiceFrameX = 0;
    player->anim[player->state].indiceFrameY = 0; 
    }

void Render_Player(Player *player) {

    if(player->anim[player->state].spriteSheet.id > 0){
        Vector2 posicaoVisual = player->position;
        posicaoVisual.y -= 32.0f; //offset

        bool virar = (player->PlayerDirection == -1);

        Render_DrawAnim(player->anim[player->state], posicaoVisual, virar); //Carregar protagonista
    }

    //DrawCircleV(player->position, 5.0f, GOLD); Debug
}

void Render_UpdateCamera(Camera2D *camera, Player *player, int width, int height) {
    camera->offset = (Vector2){ width/2.0f, height/2.0f };
    camera->target = player->position;
}
//spriets dos inimigos

void Render_LoadAssets(void){
    texBoar = LoadTexture("assets/sprites/enemies/Boar/Walk/Walk-Base-Sheet-White.png"); 
    texBee = LoadTexture("assets/sprites/enemies/Small Bee/Fly/Fly-Sheet.png");
    texSnail = LoadTexture("assets/sprites/enemies/Snail/walk-Sheet.png");
}

void Render_UnloadAssets(void){
    UnloadTexture(texBoar); 
    UnloadTexture(texBee);
    UnloadTexture(texSnail);
}
void Render_ConfigEnemy(Enemy *e, EnemyType type){
    e->useTexture = 1;

    e->rowIdle = 0;
    e->rowWalk = 0;
    e-> rowRun = 0;
    e-> rowAttack = 0;

    switch (type) {
        case ENEMY_TYPE_BOAR:
            e->texture = texBoar;
            e->frameWidth = 48;   
            e->frameHeight = 32; 
            e->width = 30;  //novos tamanhos 
            e->height = 32; // novos tamanhos
            e->maxFramesIdle = 6; 
            e->maxFramesWalk = 6;
            e->maxFramesRun = 6;
            e->frameTime = 0.15f;
            break;

        case ENEMY_TYPE_SMALL_BEE:
            e->texture = texBee;
            e->frameWidth = 64; 
            e->frameHeight = 64;
            e->width = 40;
            e->height = 40; 
            e->maxFramesIdle = 4;
            e->maxFramesWalk = 4;
            e->frameTime = 0.1f;
            break;

        case ENEMY_TYPE_SNAIL: //Problemas com a sprite do caracol
            e->texture = texSnail;
            e->frameWidth = 32;
            e->frameHeight = 48;
            e->width = 28;
            e->height = 30;
            e->maxFramesWalk = 8;
            e->frameTime = 0.75f;
            break;
    }
}

void Render_Enemy(Enemy *e){

    if(!e->active) return;

    if(!e->useTexture){ // debug
        Rectangle r = {e->position.x, e->position.y - e->height, e->width,e->height};
        DrawRectangleRec(r, PURPLE);
        return;
    }

    int row = 0; // logica das sprites
    switch(e->state){
        case ENEMY_STATE_IDLE:   row = e->rowIdle;   break;
        case ENEMY_STATE_WALK:   row = e->rowWalk;   break;
        case ENEMY_STATE_RUN:    row = e->rowRun;    break;
        case ENEMY_STATE_ATTACK: row = e->rowAttack; break;
        default: row = e->rowIdle; break;
    }
    Rectangle src = {e->frame * e->frameWidth, row * e->frameHeight, e->frameWidth * e->direction * -1, e->frameHeight}; //recorte
    //posicionar
    float spriteOffsetx = (e->frameWidth - e->width) / 2.0f;
    float spriteOffsety = (e->frameHeight - e->height);

    Rectangle dst ={e->position.x - spriteOffsetx, e->position.y - e->frameHeight, e->frameWidth,e->frameHeight};
    
    DrawTexturePro(e->texture, src, dst, (Vector2){0, 0}, 0.0f, WHITE); // desenha
}

AnimacaoSpritesheet Render_CreateAnim(const char *path, int fX, int fY, float fps, int w, int h, bool loopar, bool menu) { //Carrega Sprite
    AnimacaoSpritesheet anim = {0};
    anim.spriteSheet = LoadTexture(path);
    anim.framesX = fX;
    anim.framesY = fY;
    anim.tempoPorFrame = 1.0f / fps;
    anim.temporizador = anim.tempoPorFrame;
    anim.larguraFrame = w;
    anim.alturaFrame = h;
    anim.inverteAnimacao = false;
    anim.loop = loopar;
    anim.ismenu = menu;
    return anim;
}

void Render_UpdateAnim(AnimacaoSpritesheet *anim, float dt) { //motor dos sprites
    anim->temporizador -= dt;
    if (anim->temporizador < 0) {
        anim->temporizador = anim->tempoPorFrame;
        
        if (!anim->inverteAnimacao) {   
            anim->indiceFrameX++;
            if (anim->indiceFrameX >= anim->framesX) {
                if(anim->loop){
                    anim->indiceFrameX = 0;
                    if(anim->ismenu) anim->indiceFrameY++;
                }
                else{
                    anim->indiceFrameX = anim->indiceFrameX - 1;
                    anim->final = true;
                }
            }
        } else {
            anim->indiceFrameX--;
            if (anim->indiceFrameX < 0) {
                anim->indiceFrameX = anim->framesX - 1;
                if(anim->ismenu) anim->indiceFrameY--;
            }
        }
        
        // Check boundaries for loop/invert
        if(anim->loop){
        if (anim->indiceFrameX >= anim->framesX - 1 && anim->indiceFrameY >= anim->framesY - 1)
            anim->inverteAnimacao = true;
        else if (anim->indiceFrameX <= 0 && anim->indiceFrameY <= 0)
            anim->inverteAnimacao = false;
        }
    }
}

void Render_DrawAnim(AnimacaoSpritesheet anim, Vector2 centroDestino, bool virar) {
    Rectangle origem = {
        anim.indiceFrameX * anim.larguraFrame,
        anim.indiceFrameY * anim.alturaFrame,
        anim.larguraFrame,
        anim.alturaFrame
    };

    if (virar && !anim.ismenu) {
        origem.width *= -1; 
    }

    Rectangle destino = {
        centroDestino.x, centroDestino.y,
        fabsf((float)origem.width),  
        fabsf((float)origem.height)
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