#ifndef ENTITIES_H
#define ENTITIES_H

#include "../data/entity_types.h"

void Entities_ProcessPlayerInput(Player *player, float dt);
Enemy Enemy_Create(Vector2 pos, float minX, float maxX, float speed);


#endif