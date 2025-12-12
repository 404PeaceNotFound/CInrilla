#ifndef SYSTEMS_H
#define SYSTEMS_H

#include "../data/entity_types.h"
#include "../data/map_data.h"
#include <raylib.h>

// --- Physics ---
// Atualiza física do player baseada no mapa de tiles
void Physics_UpdatePlayer(Player *player, GameMap* map, float dt);
// Atualiza física do inimigo baseada no mapa de tiles
void Physics_UpdateEnemy(Enemy *enemy, GameMap* map, float dt);
// entities.h
void Enemy_UpdateAnim(Enemy *e, float dt);

// --- Render & Animation ---
void Render_LoadAssets(void);
void Render_UnloadAssets(void);

// Inicializa dados do player (sons, anims)
void initPlayer(Player *player);

// Renderização principal
void Render_Map(GameMap* map);
void Render_Player(Player *player);
void Render_Enemy(Enemy *e);

// Câmera
void Render_UpdateCamera(Camera2D *camera, Player *player, GameMap* map, int width, int height);

// Configuração de Inimigos
void Render_ConfigEnemy(Enemy *e, EnemyType type);

// --- Animation System ---
AnimacaoSpritesheet Render_CreateAnim(const char *path, int fX, int fY, float fps, int w, int h, bool loopar, bool menu);
void Render_UpdateAnim(AnimacaoSpritesheet *anim, float dt);
void Render_DrawAnim(AnimacaoSpritesheet anim, Vector2 pos, bool virar);
void Render_UnloadAnim(AnimacaoSpritesheet *anim);

#endif