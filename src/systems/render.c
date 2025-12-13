#include <raylib.h>
#include <raymath.h> 
#include "systems.h"
#include "../config/config.h"
#include <stdlib.h> // Para abs/fabs

static Texture2D texBoar;
static Texture2D texBee;
static Texture2D texSnail;
static Texture2D background;

// ============================================================================
// SISTEMA DE MAPA
// ============================================================================
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

// ============================================================================
// SISTEMA DE ANIMAÇÃO (Animation Core)
// ============================================================================
AnimacaoSpritesheet Render_CreateAnim(const char *path, int fX, int fY, float fps, int w, int h, bool loopar, bool menu) {
    AnimacaoSpritesheet anim = {0};
    if (FileExists(path)) {
        anim.spriteSheet = LoadTexture(path);
    } else {
        // Fallback ou Log de erro se quiser
    }
    anim.framesX = fX;
    anim.framesY = fY;
    anim.tempoPorFrame = (fps > 0) ? 1.0f / fps : 0.0f;
    anim.temporizador = anim.tempoPorFrame;
    anim.larguraFrame = w;
    anim.alturaFrame = h;
    anim.inverteAnimacao = false;
    anim.loop = loopar;
    anim.ismenu = menu;
    anim.indiceFrameX = 0;
    anim.indiceFrameY = 0;
    anim.final = false;
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
            if (anim->indiceFrameY >= anim->framesY) anim->indiceFrameY = 0;
        }
    }
}

void Render_DrawAnim(AnimacaoSpritesheet anim, Vector2 centroDestino, bool virar) {
    if (anim.spriteSheet.id == 0) return; // Segurança

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
        fabsf((float)anim.larguraFrame),  
        fabsf((float)anim.alturaFrame)
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

// ============================================================================
// SISTEMA DO PLAYER
// ============================================================================
void Render_Player(Player *player) {
    // Verifica se a textura é válida
    if(player->anim[player->state].spriteSheet.id > 0){
        
        Vector2 posicaoVisual = player->position;
        
        // 1. Offset Global (para centralizar o boneco em pé)
        posicaoVisual.y -= player->renderoffsetY;

        // 2. CORREÇÃO DA MORTE (O Pulo do Gato)
        // Se estiver morto, precisamos "desfazer" parte da subida acima
        // ou empurrar ele mais para baixo.
        if (player->state == PlayerDead) {
            // Aumente este valor até ele tocar o chão. 
            // Como você subiu 32 ali em cima, somar uns 15 ou 20 deve resolver.
            posicaoVisual.y += 20.0f; 
            
            // Opcional: Ajuste lateral se ele cair desalinhado
            // posicaoVisual.x -= 5.0f; 
        }

        bool virar = (player->PlayerDirection == -1);
        
        // Efeito Visual de Dano (Piscar)
        if (player->hurtTimer > 0) {
            // Pisca a cada 0.1s (lógica simplificada)
            if (((int)(GetTime() * 10) % 2) == 0) return; 
        }

        Render_DrawAnim(player->anim[player->state], posicaoVisual, virar);
    }
}

void Render_UpdatePlayerAnim(Player *player, float dt) {
    Render_UpdateAnim(&player->anim[player->state], dt);
}

void initPlayer(Player *player) {
    player->state = PlayerIdle;
    player->isatk = false;
    player->PlayerDirection = 1;
    player->damage = 2;
    player->health = 10;
    player->hurtTimer = 0;

    // Carregar Sons (Verifica se existem para evitar erros)
    if(FileExists("assets/sounds/Player/Run.mp3")) player->soundPlayer.Run = LoadSound("assets/sounds/Player/Run.mp3");
    if(FileExists("assets/sounds/Player/Atk.mp3")) player->soundPlayer.Atk = LoadSound("assets/sounds/Player/Atk.mp3");
    if(FileExists("assets/sounds/Player/Jump.mp3")) player->soundPlayer.Jump = LoadSound("assets/sounds/Player/Jump.mp3");
    

    // Configurar Animações
    player->anim[PlayerIdle] = Render_CreateAnim("assets/sprites/character/Idle/Idle-Sheet.png", 4, 1, 6, 64, 64, true, false);
    player->anim[PlayerRun] = Render_CreateAnim("assets/sprites/character/Run/Run-Sheet.png", 8, 1, 7, 80, 64, true, false);
    player->anim[PlayerJump] = Render_CreateAnim("assets/sprites/character/Jumlp-All/Jump-All-Sheet.png", 15, 1, 6, 64, 64, true, false);
    player->anim[PlayerAtk] = Render_CreateAnim("assets/sprites/character/Attack-01/Attack-01-Sheet.png", 8, 1, 8, 96, 64, false, false);
    player->anim[PlayerDead] = Render_CreateAnim("assets/sprites/character/Dead/Dead-Sheet.png", //Atacar
        8,   // framesX 
        1,   // framesY
        8,   // fps (0 para estático)
        80,  // largura do frame
        64,   // altura do frame
        false, false);
}

// ============================================================================
// SISTEMA DE INIMIGOS
// ============================================================================
void Render_LoadAssets(void) {
    if(FileExists("assets/sprites/enemies/Boar/Walk/Walk-Base-Sheet-White.png"))
        texBoar = LoadTexture("assets/sprites/enemies/Boar/Walk/Walk-Base-Sheet-White.png"); 
    
    if(FileExists("assets/sprites/enemies/Small Bee/Fly/Fly-Sheet.png"))
        texBee = LoadTexture("assets/sprites/enemies/Small Bee/Fly/Fly-Sheet.png");
    
    if(FileExists("assets/sprites/enemies/Snail/walk-Sheet.png"))
        texSnail = LoadTexture("assets/sprites/enemies/Snail/walk-Sheet.png");

    if(FileExists("assets/sprites/background/sky.png")) {
        background = LoadTexture("assets/images/background/Background (2).png");
    }
}

void Render_UnloadAssets(void) {
    if(texBoar.id != 0) UnloadTexture(texBoar); 
    if(texBee.id != 0) UnloadTexture(texBee);
    if(texSnail.id != 0) UnloadTexture(texSnail);
    if(background.id != 0) UnloadTexture(background);
}

void Render_ConfigEnemy(Enemy *e, EnemyType type) {
    e->useTexture = true;
    e->rowIdle = 0; e->rowWalk = 0; e->rowRun = 0; e->rowAttack = 0;
    e->frameTimer = 0.0f;

    switch (type) {
        case ENEMY_TYPE_BOAR:
            e->texture = texBoar;
            e->frameWidth = 48; e->frameHeight = 32; 
            e->width = 30; e->height = 32;
            e->maxFramesIdle = 6; e->maxFramesWalk = 6; e->maxFramesRun = 6;
            e->maxFramesAttack = 6;
            e->frameTime = 0.15f;
            break;
        case ENEMY_TYPE_SMALL_BEE:
            e->texture = texBee;
            e->frameWidth = 64; e->frameHeight = 64;
            e->width = 40; e->height = 40; 
            e->maxFramesIdle = 4; e->maxFramesWalk = 4;
            e->maxFramesAttack = 4;
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

    // --- DEBUG: CAIXA DE COLISÃO ---
    // Desenha o quadrado vermelho SEMPRE (para debug), ou apenas se textura falhar
    // Descomente a linha abaixo para ver onde as hitboxes estão:
    // DrawRectangleLines(e->position.x, e->position.y - e->height, e->width, e->height, RED);

    // Verificação de Textura
    if(!e->useTexture || e->texture.id == 0){ 
        DrawRectangle(e->position.x, e->position.y - e->height, e->width, e->height, MAROON);
        return;
    }

    int row = 0;
    switch(e->state){
        case ENEMY_STATE_IDLE:   row = e->rowIdle;   break;
        case ENEMY_STATE_WALK:   row = e->rowWalk;   break;
        case ENEMY_STATE_RUN:    row = e->rowRun;    break;
        case ENEMY_STATE_ATTACK: row = e->rowAttack; break;
        default: row = 0; break;
    }

    // 1. CORREÇÃO DA DIREÇÃO (Evita sumir se direction for 0)
    float dir = (float)e->direction;
    if (dir == 0) dir = 1.0f; // Se for 0, assume direita

    Rectangle src = {
        e->frame * e->frameWidth, 
        row * e->frameHeight, 
        e->frameWidth * dir * -1, // Inverte para Raylib
        (float)e->frameHeight
    };
    
    // 2. CORREÇÃO DE ESCALA (Para o Boss Gigante)
    // Em vez de usar frameWidth/Height fixos, calculamos uma escala
    // baseada no tamanho da Hitbox do inimigo.
    
    // Fator de escala (Se hitbox cresceu, sprite cresce)
    // Usamos a altura como referência para manter proporção
    float scale = e->height / (float)e->frameHeight; 
    
    // Se quiser manter o tamanho original para inimigos normais que têm hitbox menor que o sprite:
    // (Apenas descomente se os inimigos normais ficarem estranhos)
    // if (scale < 1.0f) scale = 1.0f; 

    float drawWidth = e->frameWidth * scale;
    float drawHeight = e->frameHeight * scale;

    // 3. POSICIONAMENTO CENTRALIZADO
    // Centraliza o sprite visualmente em relação à hitbox X
    float spriteOffsetx = (drawWidth - e->width) / 2.0f;
    
    Rectangle dst = {
        e->position.x - spriteOffsetx, 
        e->position.y - drawHeight + e->renderoffsetY, // Usa a altura desenhada + offset manual
        drawWidth, 
        drawHeight
    };
    
    DrawTexturePro(e->texture, src, dst, (Vector2){0, 0}, 0.0f, WHITE);
}

void Render_DrawBackground(int larguraTela, int alturaTela) {
    if (background.id == 0) return; // Se não carregou, não faz nada

    // Estica a imagem para caber na tela
    Rectangle source = { 0, 0, background.width, background.height };
    Rectangle dest = { 0, 0, (float)larguraTela, (float)alturaTela };
    
    DrawTexturePro(background, source, dest, (Vector2){0, 0}, 0.0f, WHITE);
}

// ============================================================================
// CAMERA
// ============================================================================
void Render_UpdateCamera(Camera2D *camera, Player *player, GameMap* map, int width, int height) {
    // Calcula o alvo ideal (onde a câmera QUERIA estar)
    Vector2 targetIdeal = player->position;
    
    // Configura um offset (meio da tela)
    camera->offset = (Vector2){ width/2.0f, height/2.0f };

    // --- A SOLUÇÃO DA TRAVADA ---
    // Em vez de atribuir direto (=), usamos Lerp (Interpolação Linear).
    // O 0.1f é a "preguiça". Quanto menor, mais suave (mas mais atrasada).
    // Teste valores entre 0.05f e 0.15f.
    
    float smoothFactor = 0.1f; 

    camera->target.x = Lerp(camera->target.x, targetIdeal.x, smoothFactor);
    camera->target.y = Lerp(camera->target.y, targetIdeal.y, smoothFactor);
    
    // ----------------------------

    // Travas do mapa (Clamp)
    if (!map->loaded) return;

    // ... (Mantenha seu código de Clamp/Limites do mapa aqui, 
    // mas aplique sobre camera->target atualizado pelo Lerp) ...
    
    float minX = width/2.0f / camera->zoom;
    float maxX = (map->width * map->tileWidth) - minX;
    float minY = height/2.0f / camera->zoom;
    float maxY = (map->height * map->tileHeight) - minY;
    
    camera->target.x = Clamp(camera->target.x, minX, maxX);
    camera->target.y = Clamp(camera->target.y, minY, maxY);
}


void Render_UpdateEnemyAnim(Enemy *e, float dt) {
    if (!e->active) return;

    // --- CORREÇÃO AQUI ---
    // Usamos frameTimer (o acumulador) e não frameTime (a duração fixa)
    e->frameTimer += dt;

    // Verificamos se o acumulador passou da duração limite
    if (e->frameTimer >= e->frameTime) {
        
        // Resetamos o acumulador (pode subtrair frameTime para maior precisão)
        e->frameTimer = 0.0f; 
        
        // Avança o quadro
        e->frame++;

        // Descobre quantos frames tem a animação atual
        int maxFrames = 0;
        switch (e->state) {
            case ENEMY_STATE_IDLE:   maxFrames = e->maxFramesIdle;   break;
            case ENEMY_STATE_WALK:   maxFrames = e->maxFramesWalk;   break;
            case ENEMY_STATE_RUN:    maxFrames = e->maxFramesRun;    break;
            case ENEMY_STATE_ATTACK: maxFrames = e->maxFramesAttack; break;
            default: maxFrames = 1; break;
        }

        // Se passar do limite, volta para o frame 0 (Loop)
        if (e->frame >= maxFrames) {
            e->frame = 0;
        }
    }
}

