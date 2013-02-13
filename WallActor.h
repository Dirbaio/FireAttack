#ifndef WALLACTOR_H
#define WALLACTOR_H

#include "Actor.h"

class WallActor : public Actor
{
public:
    WallActor(Scene* sc, float x, float v);
};

#endif // WALLACTOR_H
