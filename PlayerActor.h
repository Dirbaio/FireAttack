#ifndef PLAYERACTOR_H
#define PLAYERACTOR_H

#include "Actor.h"
#include "FireActor.h"
#include "Input.h"

class GameScene;

struct PlayerConfig
{
    vec3 initPos;
    vec3 col1, col2, col3, col4, col5, col6;
    vector<sf::Keyboard::Key> keyMap;
};

class PlayerActor : public Actor
{
private:
    float bounce_cooldown, bounce_cooldown_min, bounce_factor_x, bounce_factor_y;

public:

    float size;

    GameScene* gsc;
    PlayerActor(GameScene* sc, PlayerConfig *config);
    Texture* playerTex;

    PlayerConfig cfg;
    Input* input;

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
