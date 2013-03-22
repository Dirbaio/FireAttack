#ifndef ENEMY_H
#define ENEMY_H

#include "PlayerActor.h"
#include "Actor.h"
#include "GameScene.h"

class Enemy : public Actor
{
private:
    PlayerActor* player;
    GameScene* gsc;
    float size;

public:
    Enemy(GameScene* sc, float size, vec3 pos);
    virtual void update();
    virtual void render();
};

#endif // ENEMY_H
