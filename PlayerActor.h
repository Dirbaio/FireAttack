#ifndef PLAYERACTOR_H
#define PLAYERACTOR_H

#include "Actor.h"
#include "FireActor.h"
#include "Input.h"
#include "Pointer.h"

class Input;
class GameScene;


class PlayerActor : public Actor
{
private:
    float bounce_cooldown, bounce_cooldown_min, bounce_factor_x, bounce_factor_y;
    bool canDash;
    float dashCooldownTime, dashCooldownTimeMax;
    float shootCooldownTime, shootCooldownTimeMax;
    Pointer* ptr;

public:

    float size;

    GameScene* gsc;
    Texture* playerTex;

    PlayerConfig cfg;
    Input* input;

    ParticleEmitter* sizeEmitter;
    FireActor* test;

    int numPlayer;

    PlayerActor(GameScene* sc, PlayerConfig config, int numPlayer);
    virtual ~PlayerActor();

    virtual void update();
    virtual void render();
    virtual bool renderParticle(Particle &p);

    void explode();
    void explodeWater();

    virtual void collided(Actor *b);
    virtual bool collidedWithGround();

};

#endif // PLAYERACTOR_H
