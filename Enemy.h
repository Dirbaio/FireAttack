#ifndef ENEMY_H
#define ENEMY_H

#include "Actor.h"
#include "PlayerActor.h"

class GameScene;

class Enemy : public Actor
{
private:
    float size;

public:
    GameScene* gsc;
    PlayerActor* player;

    Enemy(GameScene* sc, float size, vec3 pos, vec3 col);
    virtual void update();
    virtual void render();
};

#endif // ENEMY_H
