#ifndef POINTER_H
#define POINTER_H

class Pointer;

#include "GameScene.h"
#include "Actor.h"
#include "PlayerActor.h"

class Pointer : public Actor
{
    GameScene* gsc;
    PlayerActor* pl;
public:
    Pointer(GameScene* sc, PlayerActor* pl);
    virtual void update();
    virtual bool renderParticle(Particle &p);
};

#endif // POINTER_H
