#ifndef Collision_H
#define Collision_H 1

#include "object.h"

bool collisionEffect(Object* o1, Object* o2);
void collisionImmovable(Object *movable, Object *imovable);
void collisionObstruse(Object *o1, Object *o2);
void collisionAttack(Object *o1, Object *o2);

#endif
