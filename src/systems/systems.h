#ifndef SYSTEMS_H
#define SYSTEMS_H

#include "../data/entity_types.h"
#include "../data/map_data.h"
#include <raylib.h>

// --- Physics (Física) ---
void Physics_UpdatePlayer(Player *player, GameMap* map, float dt);
void Physics_UpdateEnemy(Enemy *enemy, GameMap* map, float dt);

// --- Render & Animation (Gráficos) ---
void Render_LoadAssets(void);
void Render_UnloadAssets(void);

// Player
void initPlayer(Player *player);
void Render_Player(Player *player);
void Render_UpdatePlayerAnim(Player *player, float dt);

// Map & Camera
void Render_Map(GameMap* map);
void Render_DrawBackground(int larguraTela, int alturaTela);
void Render_UpdateCamera(Camera2D *camera, Player *player, GameMap* map, int width, int height);

// Enemy
void Render_ConfigEnemy(Enemy *e, EnemyType type);
void Render_Enemy(Enemy *e);

// --- Animation System (Sistema de Animação Genérico) ---
AnimacaoSpritesheet Render_CreateAnim(const char *path, int fX, int fY, float fps, int w, int h, bool loopar, bool menu);
void Render_UpdateAnim(AnimacaoSpritesheet *anim, float dt);
void Render_DrawAnim(AnimacaoSpritesheet anim, Vector2 pos, bool virar);
void Render_UnloadAnim(AnimacaoSpritesheet *anim);

#endif