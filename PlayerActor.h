#ifndef PLAYERACTOR_H
#define PLAYERACTOR_H

#include "Actor.h"
#include "FireActor.h"
class GameScene;

class PlayerActor : public Actor
{
    public:

    float size;

    GameScene* gsc;
    PlayerActor(GameScene* sc);
    Texture* playerTex;

    ParticleEmitter* sizeEmitter;
    FireActor* test;
    bool wasMouseDown;
    float mouseDownTime;

    virtual void update();
    virtual void render();
    void explode();

    virtual bool collided(Actor *b);
    virtual bool collidedWithGround();

};

#endif // PLAYERACTOR_H
