#include <raylib.h>
#include <raymath.h> 
#include "systems.h"
#include "../config/config.h"

static Texture2D texBoar;
static Texture2D texBee;
static Texture2D texSnail;

// --- Map Rendering ---
static void DrawLayer(MapLayer* layer, GameMap* map) {
    if (!layer->data || !map->texture.id) return;

    for (int y = 0; y < layer->height; y++) {
        for (int x = 0; x < layer->width; x++) {
            int index = y * layer->width + x;
            int gid = layer->data[index];

            if (gid == 0) continue;

            int tilesetIndex = gid - map->firstGid;
            if (tilesetIndex < 0) continue;

            int textureX = (tilesetIndex % map->columns) * map->tileWidth;
            int textureY = (tilesetIndex / map->columns) * map->tileHeight;

            Rectangle source = { 
                (float)textureX, (float)textureY, 
                (float)map->tileWidth, (float)map->tileHeight 
            };
            Vector2 position = { 
                (float)x * map->tileWidth, (float)y * map->tileHeight 
            };

            DrawTextureRec(map->texture, source, position, WHITE);
        }
    }
}

void Render_Map(GameMap* map) {
    if (!map->loaded) return;

    for (int i = 0; i < map->layersCount; i++) {
        if (map->layers[i].isVisible) {
            DrawLayer(&map->layers[i], map);
        }
    }
}

// --- Player Rendering ---
void Render_Player(Player *player) {
    if(player->anim[player->state].spriteSheet.id > 0){
        Vector2 posicaoVisual = player->position;
        posicaoVisual.y -= 32.0f; // Offset visual (sprite maior que hitbox)

        bool virar = (player->PlayerDirection == -1);
        Render_DrawAnim(player->anim[player->state], posicaoVisual, virar);
    }
}

void initPlayer(Player *player) {
    player->state = PlayerIdle;
    player->isatk = false;
    player->PlayerDirection = 1;
    player->damage = 5;
    player->health = 15;

    // Carregar Sons
    player->soundPlayer.Run = LoadSound("assets/sounds/Player/Run.mp3");
    player->soundPlayer.Atk = LoadSound("assets/sounds/Player/Atk.mp3");
    player->soundPlayer.Jump = LoadSound("assets/sounds/Player/Jump.mp3");

    // Configurar Animações
    player->anim[PlayerIdle] = Render_CreateAnim("assets/sprites/character/Idle/Idle-Sheet.png", 4, 1, 6, 64, 64, true, false);
    player->anim[PlayerRun] = Render_CreateAnim("assets/sprites/character/Run/Run-Sheet.png", 8, 1, 7, 80, 64, true, false);
    player->anim[PlayerJump] = Render_CreateAnim("assets/sprites/character/Jumlp-All/Jump-All-Sheet.png", 15, 1, 6, 64, 64, true, false);
    player->anim[PlayerAtk] = Render_CreateAnim("assets/sprites/character/Attack-01/Attack-01-Sheet.png", 8, 1, 8, 96, 64, false, false);

    player->anim[player->state].indiceFrameX = 0;
    player->anim[player->state].indiceFrameY = 0; 
}

// --- Camera ---
void Render_UpdateCamera(Camera2D *camera, Player *player, GameMap* map, int width, int height) {
    camera->offset = (Vector2){ width/2.0f, height/2.0f };
    camera->target = player->position;

    if (!map->loaded) return;

    // Clamping da Câmera
    float minX_world = 0.0f;
    float minY_world = 0.0f;
    float maxX_world = (float)map->width * map->tileWidth;
    float maxY_world = (float)map->height * map->tileHeight;

    float targetMinX = minX_world + width / 2.0f / camera->zoom; 
    float targetMinY = minY_world + height / 2.0f / camera->zoom;
    float targetMaxX = maxX_world - width / 2.0f / camera->zoom; 
    float targetMaxY = maxY_world - height / 2.0f / camera->zoom;

    if (targetMinX > targetMaxX) targetMinX = targetMaxX = (minX_world + maxX_world) / 2.0f;
    if (targetMinY > targetMaxY) targetMinY = targetMaxY = (minY_world + maxY_world) / 2.0f;

    camera->target.x = Clamp(camera->target.x, targetMinX, targetMaxX);
    camera->target.y = Clamp(camera->target.y, targetMinY, targetMaxY);
}

// --- Enemy Rendering ---
void Render_LoadAssets(void) {
    texBoar = LoadTexture("assets/sprites/enemies/Boar/Walk/Walk-Base-Sheet-White.png"); 
    texBee = LoadTexture("assets/sprites/enemies/Small Bee/Fly/Fly-Sheet.png");
    texSnail = LoadTexture("assets/sprites/enemies/Snail/walk-Sheet.png");
}

void Render_UnloadAssets(void) {
    UnloadTexture(texBoar); 
    UnloadTexture(texBee);
    UnloadTexture(texSnail);
}

void Render_ConfigEnemy(Enemy *e, EnemyType type) {
    e->useTexture = 1;
    e->rowIdle = 0; e->rowWalk = 0; e->rowRun = 0; e->rowAttack = 0;

    switch (type) {
        case ENEMY_TYPE_BOAR:
            e->texture = texBoar;
            e->frameWidth = 48; e->frameHeight = 32; 
            e->width = 30; e->height = 32;
            e->maxFramesIdle = 6; e->maxFramesWalk = 6; e->maxFramesRun = 6;
            e->frameTime = 0.15f;
            break;
        case ENEMY_TYPE_SMALL_BEE:
            e->texture = texBee;
            e->frameWidth = 64; e->frameHeight = 64;
            e->width = 40; e->height = 40; 
            e->maxFramesIdle = 4; e->maxFramesWalk = 4;
            e->frameTime = 0.1f;
            break;
        case ENEMY_TYPE_SNAIL:
            e->texture = texSnail;
            e->frameWidth = 32; e->frameHeight = 48;
            e->width = 28; e->height = 30;
            e->maxFramesWalk = 8;
            e->frameTime = 0.75f;
            break;
    }
}

void Render_Enemy(Enemy *e) {
    if(!e->active) return;

    if(!e->useTexture){ 
        Rectangle r = {e->position.x, e->position.y - e->height, e->width,e->height};
        DrawRectangleRec(r, PURPLE);
        return;
    }

    int row = 0;
    switch(e->state){
        case ENEMY_STATE_IDLE:   row = e->rowIdle;   break;
        case ENEMY_STATE_WALK:   row = e->rowWalk;   break;
        case ENEMY_STATE_RUN:    row = e->rowRun;    break;
        case ENEMY_STATE_ATTACK: row = e->rowAttack; break;
        default: row = e->rowIdle; break;
    }

    Rectangle src = {
        e->frame * e->frameWidth, 
        row * e->frameHeight, 
        e->frameWidth * e->direction * -1, 
        e->frameHeight
    };
    
    float spriteOffsetx = (e->frameWidth - e->width) / 2.0f;
    
    Rectangle dst = {
        e->position.x - spriteOffsetx, 
        e->position.y - e->frameHeight, 
        e->frameWidth, 
        e->frameHeight
    };
    
    DrawTexturePro(e->texture, src, dst, (Vector2){0, 0}, 0.0f, WHITE);
}

// --- Animation Core ---
AnimacaoSpritesheet Render_CreateAnim(const char *path, int fX, int fY, float fps, int w, int h, bool loopar, bool menu) {
    AnimacaoSpritesheet anim = {0};
    anim.spriteSheet = LoadTexture(path);
    anim.framesX = fX;
    anim.framesY = fY;
    anim.tempoPorFrame = (fps > 0) ? 1.0f / fps : 0.0f;
    anim.temporizador = anim.tempoPorFrame;
    anim.larguraFrame = w;
    anim.alturaFrame = h;
    anim.inverteAnimacao = false;
    anim.loop = loopar;
    anim.ismenu = menu;
    return anim;
}

void Render_UpdateAnim(AnimacaoSpritesheet *anim, float dt) {
    if (anim->tempoPorFrame <= 0) return;
    
    anim->temporizador -= dt;
    if (anim->temporizador < 0) {
        anim->temporizador = anim->tempoPorFrame;
        
        if (!anim->inverteAnimacao) {   
            anim->indiceFrameX++;
            if (anim->indiceFrameX >= anim->framesX) {
                if(anim->loop){
                    anim->indiceFrameX = 0;
                    if(anim->ismenu) anim->indiceFrameY++;
                } else {
                    anim->indiceFrameX = anim->framesX - 1;
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

        if(anim->loop && anim->ismenu) {
            if (anim->indiceFrameY >= anim->framesY) anim->indiceFrameY = 0; // Proteção extra
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