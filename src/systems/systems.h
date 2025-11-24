#ifndef SYSTEMS_H
#define SYSTEMS_H

#include "../data/entity_types.h"
#include "../data/map_data.h"
#include "enemy_system.h"

// Physics
void Physics_UpdatePlayer(Player *player, EnvItem *envItems, int envLength, float dt);
// EPysics enemy
void Physics_UpdateEnemy(Enemy *enemy, EnvItem *envItems, int envLength, float dt);
// Render & Animation
void initPlayer(Player *player);
void Render_Map(EnvItem *envItems, int envLength);
void Render_Player(Player *player);
void Render_UpdateCamera(Camera2D *camera, Player *player, int width, int height);

// Animation System
AnimacaoSpritesheet Render_CreateAnim(const char *path, int fX, int fY, float fps, int w, int h, bool loopar, bool menu);
void Render_UpdateAnim(AnimacaoSpritesheet *anim, float dt);
void Render_DrawAnim(AnimacaoSpritesheet anim, Vector2 pos, bool virar);
void Render_UnloadAnim(AnimacaoSpritesheet *anim);

#endif