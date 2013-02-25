#ifndef ENEMY_H
#define ENEMY_H

#include "PlayerActor.h"
#include "Actor.h"
#include "GameScene.h"

class Enemy : public Actor
{
private:
    PlayerActor* player;
    GameScene* sc;
public:
    Enemy(GameScene* sc);
    virtual void update();
    virtual void render();
};

#endif // ENEMY_H
