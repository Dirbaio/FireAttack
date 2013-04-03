#ifndef SHOOTERENEMY_H
#define SHOOTERENEMY_H

#include "Enemy.h"

class GameScene;

class ShooterEnemy : public Enemy
{
private:
    float shootTimer;

public:

    ShooterEnemy(GameScene* sc, vec3 pos);

    virtual void update();
    virtual void render();
};

#endif // SHOOTERENEMY_H
