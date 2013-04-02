#ifndef EXPLOSIONACTOR_H
#define EXPLOSIONACTOR_H

#include "Actor.h"
#include "Scene.h"
#include <list>

class ExplosionActor : public Actor
{
private:
    float timer;
    float force;
    bool destroys;
    float radio;

public:
    ExplosionActor(Scene *sc, vec3 pos, float force, bool explodes, bool destroys);

    virtual void update();
};

#endif // EXPLOSIONACTOR_H
