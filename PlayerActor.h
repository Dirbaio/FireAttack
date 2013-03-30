#ifndef PLAYERACTOR_H
#define PLAYERACTOR_H

#include "Actor.h"
#include "FireActor.h"
class GameScene;

class PlayerActor : public Actor
{
private:
    float bounce_cooldown, bounce_cooldown_min, bounce_factor_x, bounce_factor_y;

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
    void explodeWater();

    virtual void collided(Actor *b);
    virtual bool collidedWithGround();

};

#endif // PLAYERACTOR_H
